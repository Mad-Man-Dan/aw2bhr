#!/usr/bin/env python3
"""MCP server exposing the aw2bhr decomp corpus to Claude Code subagents.

Subagents doing matching work need to ask the same handful of questions over and
over: what should I work on next, what does this function's assembly look like,
who calls it, what globals does it touch. Shelling out to the CLI tools and
parsing stdout for each of those is friction that multiplies across a fleet, so
they are exposed here as typed tools instead.

Every result is capped and compact on purpose -- a subagent's context is the
scarce resource, so tools return the smallest thing that answers the question
and say explicitly when output was truncated.

Run directly for stdio transport:
    python tools/mcp_server.py
"""

import json
import os
import re
import subprocess
import sys
from typing import Any

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import agbenv  # noqa: E402
import awlib  # noqa: E402
from mcp.server.fastmcp import FastMCP  # noqa: E402

mcp = FastMCP("aw2bhr-decomp")

FUNCTIONS_JSON = os.path.join(awlib.DATA_DIR, "functions.json")
CALLGRAPH_JSON = os.path.join(awlib.DATA_DIR, "callgraph.json")
FE_MATCHES_JSON = os.path.join(awlib.DATA_DIR, "fe_matches.json")
MAX_ASM_LINES = 400

_cache: dict[str, Any] = {}


def _load_json(path, key):
    """Cache by mtime so a reindex is picked up without a server restart."""
    if not os.path.exists(path):
        return None
    stamp = os.path.getmtime(path)
    hit = _cache.get(key)
    if hit and hit[0] == stamp:
        return hit[1]
    with open(path, encoding="utf-8") as fh:
        data = json.load(fh)
    _cache[key] = (stamp, data)
    return data


def _functions():
    return _load_json(FUNCTIONS_JSON, "functions")


def _callgraph():
    return _load_json(CALLGRAPH_JSON, "callgraph")


def _fe_names():
    """our sub_XXXXXXXX -> the Fire Emblem decomp name it shape-matches.

    Rebuilt from the cached document rather than cached itself, so a rerun of
    fe_signatures.py is picked up by the same mtime check as everything else.
    """
    data = _load_json(FE_MATCHES_JSON, "fe")
    if data is None:
        return {}
    return {h["name"]: h["fe_name"] for h in data["matches"]}


def _asm_index():
    """name -> Function, parsed from asm/*.s. ~5MB, so parse once and hold."""
    if "asm" not in _cache:
        index = {}
        for af in awlib.load_all():
            for fn in af.funcs:
                index[fn.name] = fn
        _cache["asm"] = index
    return _cache["asm"]


ADDR_RE = re.compile(r'^(?:0x)?([0-9A-Fa-f]{6,8})$')


def _resolve(name_or_addr: str):
    """Accept 'sub_0801D390', '0x0801D390', or '801D390'."""
    records = _functions()
    if records is None:
        return None
    key = name_or_addr.strip()
    for r in records:
        if r["name"] == key:
            return r
    m = ADDR_RE.match(key)
    if m:
        want = int(m.group(1), 16)
        for r in records:
            if r["addr"] == want:
                return r
    lowered = key.lower()
    for r in records:
        if r["name"].lower() == lowered:
            return r
    return None


def _row(r):
    """Compact one-line-ish form used by every list-returning tool."""
    row = {
        "name": r["name"],
        "addr": r["addr_hex"],
        "size": r["size"],
        "mode": r["mode"],
        "difficulty": r["difficulty"],
        "status": r["status"],
        "src": r["src"],
    }
    if r.get("trivial"):
        row["trivial"] = True
    fe = _fe_names().get(r["name"])
    if fe:
        row["fe_name"] = fe
    return row


def _need_index():
    return ("data/functions.json missing -- run `python tools/index_functions.py` "
            "then `python tools/callgraph.py`")


@mcp.tool()
def list_functions(
    status: str | None = None,
    max_difficulty: float | None = None,
    min_size: int | None = None,
    max_size: int | None = None,
    leaves_only: bool = False,
    named_only: bool = False,
    unnamed_only: bool = False,
    exclude_trivial: bool = False,
    src: str | None = None,
    limit: int = 50,
) -> dict:
    """Query the function index with filters.

    status: 'asm' (not yet decompiled), 'matched', or 'nonmatching'.
    leaves_only: only functions with no outgoing calls -- these need no unknown
      callee signatures, so they are the cheapest to match first.
    unnamed_only: only anonymous sub_XXXXXXXX functions (99.6% of the corpus).
    exclude_trivial: drop `bx lr` / `movs r0, #N; bx lr` return stubs.
    Results are capped by `limit`; `total_matching` reports the unclipped count.
    """
    records = _functions()
    if records is None:
        return {"error": _need_index()}

    cg = _callgraph()
    graph = (cg or {}).get("functions", {})

    out = []
    for r in records:
        if status and r["status"] != status:
            continue
        if max_difficulty is not None and r["difficulty"] > max_difficulty:
            continue
        if min_size is not None and r["size"] < min_size:
            continue
        if max_size is not None and r["size"] > max_size:
            continue
        if named_only and not r["named"]:
            continue
        if unnamed_only and r["named"]:
            continue
        if exclude_trivial and r.get("trivial"):
            continue
        if src and r["src"] != src:
            continue
        if leaves_only and graph.get(r["name"], {}).get("fan_out", 0) != 0:
            continue
        out.append(r)

    return {
        "total_matching": len(out),
        "returned": min(len(out), limit),
        "functions": [_row(r) for r in out[:limit]],
    }


@mcp.tool()
def get_function(name_or_addr: str, include_asm: bool = False) -> dict:
    """Detail for one function: metadata, callers/callees, globals -- and the
    assembly only on request.

    Accepts a name ('sub_0801D390') or an address ('0x0801D390').

    include_asm defaults to FALSE (changed in wave 20): the common first use is
    a batch survey, and an assembly listing delivered during a survey sits in
    the agent's context for its whole lifetime and is then delivered AGAIN by
    start_function's target_asm when the matching attempt begins. Pass
    include_asm=True only when this call IS the read for the matching attempt
    and start_function will not be used.
    """
    rec = _resolve(name_or_addr)
    if rec is None:
        records = _functions()
        if records is None:
            return {"error": _need_index()}
        return {"error": f"no function matching {name_or_addr!r}"}

    result = {
        "name": rec["name"],
        "addr": rec["addr_hex"],
        "size": rec["size"],
        "mode": rec["mode"],
        "directive": rec["directive"],
        "src": rec["src"],
        "status": rec["status"],
        "kind": rec["kind"],
        "named": rec["named"],
        "difficulty": rec["difficulty"],
        "calls": rec["calls"],
        "data_refs": rec["data_refs"],
    }

    fe = _fe_names().get(rec["name"])
    if fe:
        result["fe_name"] = fe
        result["fe_note"] = (
            f"Shape-matches {fe} in the Fire Emblem decomps (shared Intelligent "
            f"Systems engine). Use their C as the starting point and their name "
            f"when this matches -- but confirm with objdiff, the signature "
            f"compares shape, not semantics.")

    cg = _callgraph()
    if cg:
        node = cg["functions"].get(rec["name"])
        if node:
            result["callers"] = node["callers"]
            result["callees"] = node["callees"]
            result["external_calls"] = node["external"]
            result["fan_in"] = node["fan_in"]
            result["depth"] = node["depth"]
            result["batch"] = node["batch"]

    if include_asm:
        fn = _asm_index().get(rec["name"])
        if fn is None:
            result["asm_error"] = "function not found in asm/*.s"
        else:
            lines = fn.lines
            result["asm_lines"] = len(lines)
            if len(lines) > MAX_ASM_LINES:
                result["asm"] = "".join(lines[:MAX_ASM_LINES])
                result["asm_truncated"] = (
                    f"showing first {MAX_ASM_LINES} of {len(lines)} lines; "
                    f"read build/functions/ for the full unit")
            else:
                result["asm"] = "".join(lines)
    return result


@mcp.tool()
def next_work(count: int = 5, max_difficulty: float | None = None,
              leaves_only: bool = True, min_size: int = 0,
              include_trivial: bool = False) -> dict:
    """Pull the next functions to attempt, in fleet scheduling order.

    This is the fleet's work queue. Ordering is by call-graph depth (callees
    before callers, so no attempt is blocked on an unknown signature), then
    easiest-first with a discount for fan-in.

    Return stubs -- `bx lr` and `movs r0, #N; bx lr` -- are excluded by default.
    There are ~100 of them and they are the easiest possible matches, but they
    carry no information about the game and are not worth an agent apiece; use
    `stub_sweep` to clear them in one pass. Set include_trivial=True to see them
    here anyway.
    """
    cg = _callgraph()
    if cg is None:
        return {"error": "data/callgraph.json missing -- run `python tools/callgraph.py`"}
    records = _functions()
    if records is None:
        return {"error": _need_index()}
    by_name = {r["name"]: r for r in records}

    picked = []
    skipped_trivial = 0
    for name in cg["queue"]:
        rec = by_name.get(name)
        if rec is None or rec["status"] != "asm":
            continue
        if rec["kind"] == "bios":
            continue          # syscall thunks are reimplemented, not decompiled
        if rec.get("trivial") and not include_trivial:
            skipped_trivial += 1
            continue
        if max_difficulty is not None and rec["difficulty"] > max_difficulty:
            continue
        if rec["size"] < min_size:
            continue
        node = cg["functions"][name]
        if leaves_only and node["fan_out"] != 0:
            continue
        row = _row(rec)
        row["fan_in"] = node["fan_in"]
        row["depth"] = node["depth"]
        row["batch"] = node["batch"]
        picked.append(row)
        if len(picked) >= count:
            break

    out = {"returned": len(picked), "functions": picked}
    if skipped_trivial:
        out["skipped_trivial"] = skipped_trivial
    return out


@mcp.tool()
def stub_sweep(limit: int = 200) -> dict:
    """Every remaining return stub, grouped by mode and instruction shape.

    These are the `bx lr`, `movs r0, #N; bx lr`, and one-instruction SVC
    wrappers the main queue skips.
    One agent can clear the whole set in a single pass: each group shares one
    body, so the work is writing the same line N times, not N separate matching
    problems. Signatures are still unknown -- `void f(void)` vs a return type
    that happens to be ignored -- so treat the bodies as a starting point that
    the objdiff score confirms. ARM stubs are reported separately with no
    suggested C body: this project's agbcc emits THUMB only, so identical
    mnemonic text does not make an ARM `bx lr` C-matchable.
    """
    records = _functions()
    if records is None:
        return {"error": _need_index()}

    asm = _asm_index()
    groups: dict[tuple[str, str], list] = {}
    for r in records:
        if not r.get("trivial") or r["status"] != "asm" or r["kind"] == "bios":
            continue
        fn = asm.get(r["name"])
        if fn is None:
            continue
        shape = " ; ".join(awlib.instructions(fn))
        groups.setdefault((fn.mode, shape), []).append(r)

    out = []
    for (mode, shape), rows in sorted(groups.items(), key=lambda kv: -len(kv[1])):
        body = None
        if mode == "THUMB":
            body = "void %s(void)\n{\n}" % "FUNC"
            m = re.match(r'^movs r0, #(\S+) ; bx lr$', shape)
            if m:
                body = "int %s(void)\n{\n    return %s;\n}" % ("FUNC", m.group(1))
            m = re.match(r'^svc #(\S+) ; bx lr$', shape)
            if m:
                body = ('void %s(void)\n{\n    asm("svc #%s");\n}'
                        % ("FUNC", m.group(1)))
        group = {
            "mode": mode,
            "asm": shape,
            "count": len(rows),
            "suggested_body": body,
            "c_matchable": mode == "THUMB",
            "functions": [r["name"] for r in rows[:limit]],
        }
        if mode != "THUMB":
            group["note"] = ("agbcc emits THUMB only; retain these ARM stubs "
                             "as assembly unless an ARM compiler route is added")
        out.append(group)

    total = sum(g["count"] for g in out)
    return {"total_stubs": total, "shapes": len(out), "groups": out}


@mcp.tool()
def search_functions(query: str, limit: int = 30) -> dict:
    """Find functions by name substring, address, or referenced global symbol.

    Searching a global (e.g. 'gDispIo') returns every function that touches it,
    which is the fastest way to find the code owning a piece of state.
    """
    records = _functions()
    if records is None:
        return {"error": _need_index()}

    q = query.strip().lower()
    by_name, by_data = [], []
    for r in records:
        if q in r["name"].lower():
            by_name.append(r)
            continue
        if any(q in d.lower() for d in r["data_refs"]):
            by_data.append(r)

    exact = _resolve(query)
    if exact and exact not in by_name:
        by_name.insert(0, exact)

    return {
        "name_matches": len(by_name),
        "data_ref_matches": len(by_data),
        "by_name": [_row(r) for r in by_name[:limit]],
        "by_data_ref": [_row(r) for r in by_data[:limit]],
    }


@mcp.tool()
def callgraph(name_or_addr: str, depth: int = 1) -> dict:
    """Callers and callees around a function, expanded `depth` hops.

    Use this to size a batch: a tight cluster is worth matching together
    because the structs worked out for one member carry to the rest.
    """
    rec = _resolve(name_or_addr)
    if rec is None:
        return {"error": f"no function matching {name_or_addr!r}"}
    cg = _callgraph()
    if cg is None:
        return {"error": "data/callgraph.json missing -- run `python tools/callgraph.py`"}

    graph = cg["functions"]
    seen = {rec["name"]}
    frontier = {rec["name"]}
    for _ in range(max(0, depth)):
        nxt = set()
        for n in frontier:
            node = graph.get(n)
            if not node:
                continue
            nxt.update(node["callers"])
            nxt.update(node["callees"])
        nxt -= seen
        seen |= nxt
        frontier = nxt
        if not frontier:
            break

    root = graph.get(rec["name"], {})
    return {
        "root": rec["name"],
        "depth": depth,
        "callers": root.get("callers", []),
        "callees": root.get("callees", []),
        "external_calls": root.get("external", []),
        "cluster_size": len(seen),
        "cluster": sorted(seen),
    }


@mcp.tool()
def fe_matches() -> dict:
    """Functions identified by shape-matching against the Fire Emblem decomps.

    Both games are Intelligent Systems GBA titles on a shared engine, so library
    code links identically. Almost all of these are the Nintendo M4A sound
    driver and a tilemap/bitmap library -- portable reference code with known-good
    C already written in the FE repos, which makes them unusually cheap matches.
    """
    data = _load_json(FE_MATCHES_JSON, "fe")
    if data is None:
        return {"error": "data/fe_matches.json missing -- run "
                         "`python tools/fe_signatures.py`"}
    return {
        "identified": len(data["matches"]),
        "eligible": data["eligible"],
        "reference_functions": data["reference_functions"],
        "matches": [
            {"name": h["name"], "addr": h["addr"], "size": h["size"],
             "fe_name": h["fe_name"], "strength": h["strength"]}
            for h in data["matches"]],
    }


@mcp.tool()
def progress() -> dict:
    """Decomp progress by function count and by code bytes."""
    records = _functions()
    if records is None:
        return {"error": _need_index()}

    total = len(records)
    total_bytes = sum(r["size"] for r in records)
    done = [r for r in records if r["status"] in ("matched", "nonmatching")]
    done_bytes = sum(r["size"] for r in done)
    game = [r for r in records if r["kind"] == "game"]
    named = [r for r in records if r["named"]]

    return {
        "functions_total": total,
        "functions_done": len(done),
        "percent_functions": round(len(done) / total * 100, 2) if total else 0,
        "bytes_total": total_bytes,
        "bytes_done": done_bytes,
        "percent_bytes": round(done_bytes / total_bytes * 100, 2) if total_bytes else 0,
        "game_functions": len(game),
        "bios_stubs": total - len(game),
        "named_functions": len(named),
        "percent_named": round(len(named) / total * 100, 2) if total else 0,
        "fe_identified": len(_fe_names()),
        "return_stubs": sum(1 for r in records
                            if r.get("trivial") and r["kind"] == "game"),
    }


def _run(args: list[str], timeout: int = 300) -> dict:
    try:
        proc = subprocess.run(
            [sys.executable, *args], cwd=awlib.REPO, capture_output=True,
            text=True, timeout=timeout, stdin=subprocess.DEVNULL)
    except subprocess.TimeoutExpired as exc:
        def tail(value, n):
            if value is None:
                return ""
            if isinstance(value, bytes):
                value = value.decode("utf-8", errors="replace")
            return value[-n:]
        return {
            "ok": False,
            "exit_code": 124,
            "stdout": tail(exc.stdout, 4000),
            "stderr": tail(exc.stderr, 2000),
            "error": f"timed out after {timeout}s",
        }
    return {
        "ok": proc.returncode == 0,
        "exit_code": proc.returncode,
        "stdout": proc.stdout[-4000:],
        "stderr": proc.stderr[-2000:],
    }


@mcp.tool()
def start_function(name_or_addr: str) -> dict:
    """Scaffold a matching attempt: target assembly, a stub, and what it touches.

    Call this first, then write C and submit it with `try_match`. The returned
    signature is inferred from register use and is often wrong -- it is a
    starting point, and try_match is what settles it.
    """
    rec = _resolve(name_or_addr)
    if rec is None:
        return {"error": f"no function matching {name_or_addr!r}"}
    res = _run(["tools/newfunc.py", rec["name"]])
    if not res["ok"]:
        return {"step": "scaffold", **res}

    work = os.path.join(awlib.REPO, "work", rec["name"])
    out = {
        "name": rec["name"],
        "addr": rec["addr_hex"],
        "size": rec["size"],
        "mode": rec["mode"],
        "work_dir": f"work/{rec['name']}",
        "calls": rec["calls"],
        "data_refs": rec["data_refs"],
    }
    fe = _fe_names().get(rec["name"])
    if fe:
        out["fe_name"] = fe
        out["fe_note"] = (f"Shape-matches {fe} in the Fire Emblem decomps -- "
                          f"start from their C for this function.")
    for key, fname in (("target_asm", "target.s"), ("stub", rec["name"] + ".c")):
        path = os.path.join(work, fname)
        if os.path.exists(path):
            out[key] = "".join(awlib.read_lines(path))
    return out


@mcp.tool()
def try_match(name_or_addr: str, c_code: str | None = None,
              show_diff: bool = True, compiler_profile: str = "configured",
              cflags_add: list[str] | None = None,
              cflags_remove: list[str] | None = None) -> dict:
    """Compile candidate C for one function and report whether it matches.

    This is the verdict the whole pipeline exists to produce. `matched` is true
    only when the compiled bytes and relocations are identical to the original
    under the selected profile. Only the default `configured` profile proves
    the integrated build is ready; temporary-profile matches are also marked
    `provisional`.

    On a miss you get the byte counts, where the first difference is, and an
    instruction-level diff. Rewrite and call again. If three attempts have not
    converged, the benchmark says a fourth rarely helps -- change approach or
    move on rather than resubmitting a near-identical body.

    compiler_profile defaults to `configured`, the only canonical verdict.
    The other named profiles are temporary experiments. A match under one is
    provisional until the coordinator records an evidence-backed override and
    re-runs the configured profile.

    cflags_add / cflags_remove change single compiler flags on top of
    compiler_profile, e.g. cflags_add=["-fno-gcse"]. The result is the
    temporary profile "custom", so a match is provisional as above.

    c_code=None re-scores the draft already in work/ without rewriting it;
    use that to try the same draft under different flags.
    """
    rec = _resolve(name_or_addr)
    if rec is None:
        return {"error": f"no function matching {name_or_addr!r}"}
    if compiler_profile not in agbenv.compiler_profiles():
        return {"error": "unknown compiler_profile %r; choose %s" %
                (compiler_profile, ", ".join(agbenv.compiler_profiles()))}

    name = rec["name"]
    work = os.path.join(awlib.REPO, "work", name)
    if not os.path.isdir(work):
        scaffold = _run(["tools/newfunc.py", name])
        if not scaffold["ok"]:
            return {"step": "scaffold", **scaffold}

    if c_code is not None:
        awlib.write_text(os.path.join(work, name + ".c"), c_code)
    elif not os.path.exists(os.path.join(work, name + ".c")):
        return {"error": f"c_code is empty and work/{name}/{name}.c does not "
                         f"exist; pass the C to score"}

    args = ["tools/trymatch.py", name]
    if compiler_profile != "configured":
        args.extend(["--profile", compiler_profile])
    for opt in cflags_add or []:
        args.append("--cflags-add=" + opt)
    for opt in cflags_remove or []:
        args.append("--cflags-remove=" + opt)
    if cflags_add or cflags_remove:
        try:
            compiler_profile = agbenv.custom_profile(
                cflags_add or [], cflags_remove or [], base=compiler_profile,
                fn=name)
        except ValueError as exc:
            return {"error": str(exc)}
    if show_diff:
        args.append("--diff")
    res = _run(args)

    byte_match = res["exit_code"] == 0
    provisional = byte_match and compiler_profile != "configured"
    out = {
        "name": name,
        "size": rec["size"],
        "compiler_profile": compiler_profile,
        "matched": byte_match,
        "provisional": provisional,
        "report": res["stdout"][-6000:],
    }
    if res["stderr"].strip():
        out["stderr"] = res["stderr"][-1500:]
    if provisional:
        out["suggested_override"] = agbenv.profile_override(compiler_profile)
        out["next"] = ("The bytes match only under a temporary profile. "
                       "Give the coordinator the profile and mechanism; do not "
                       "promote. The coordinator must record an evidence-backed "
                       "override, regenerate build/compiler-overrides.mk, and "
                       "re-run the configured verdict.")
    elif out["matched"]:
        out["next"] = ("Done -- nothing further is needed for this function. "
                       "Getting it into the ROM is a separate step run by "
                       "tools/promote.py, which re-verifies the match first; "
                       "do not edit src/ or asm/ yourself.")
    return out


@mcp.tool()
def permute(name_or_addr: str, seconds: int = 300, threads: int = 4) -> dict:
    """Brute-force register allocation with decomp-permuter. Slow; last resort.

    Use this only for a candidate that is already semantically correct and stuck
    on which register something landed in -- typically above 80%, with the same
    instructions in the same order. It cannot fix a wrong struct layout or a
    wrong control-flow shape, and running it on one wastes the whole budget.

    It starts from `work/<fn>/<fn>.perm.c` if that exists (directed search),
    else `work/<fn>/best.c`, so get the score as high as you can by hand
    first. Every result is re-checked with the same byte-level test
    `try_match` uses, because the permuter's own score only diffs objdump text.

    Blocks for up to `seconds` -- the whole run. Agents running several
    searches should use the shell instead (`python tools/permute.py <fn>
    --seconds N --threads 4 --current`, backgrounded). Returns matched=false
    if nothing matched; the report's last line says whether the draft was
    IMPROVED (kept in work/<fn>/<fn>.c) or left unchanged.
    """
    rec = _resolve(name_or_addr)
    if rec is None:
        return {"error": f"no function matching {name_or_addr!r}"}

    name = rec["name"]
    if not os.path.isdir(os.path.join(awlib.REPO, "work", name)):
        return {"error": f"no work directory for {name} -- run start_function "
                         "and get a near-miss candidate before permuting"}

    res = _run(["tools/permute.py", name, "--seconds", str(seconds),
                "-j", str(threads)], timeout=seconds + 300)
    out = {
        "name": name,
        "matched": res["exit_code"] == 0,
        "report": res["stdout"][-6000:],
    }
    if res["stderr"].strip():
        out["stderr"] = res["stderr"][-1500:]
    if not out["matched"]:
        improved = "PERMUTE IMPROVED" in res["stdout"]
        out["improved"] = improved
        out["next"] = ("The search found nothing that matches at the byte "
                       "level. " + (
                           "It DID improve the draft: the better candidate is "
                           "now work/<fn>/<fn>.c. Read what the mutation did "
                           "before building on it, then chain another run from it."
                           if improved else
                           "work/<fn>/<fn>.c is unchanged. A different starting "
                           "point (or a .perm.c directing the search) usually "
                           "helps more than a longer run."))
    return out


@mcp.tool()
def compile_probe(c_code: str, name_or_addr: str | None = None,
                  compiler_profile: str = "configured",
                  cflags_add: list[str] | None = None,
                  cflags_remove: list[str] | None = None) -> dict:
    """Compile candidate C and return the assembly agbcc produced. No verdict.

    Use this to test a hypothesis. It does not count as an attempt and does not
    touch the work directory, so you can try ten spellings of an expression for
    the cost of ten seconds and only spend a `try_match` once the assembly looks
    right. One agent matched four functions in one attempt apiece this way.

    Reading the output against the target tells you most of what a diff would,
    without needing a candidate good enough to score.

    cflags_add / cflags_remove change single compiler flags, as in try_match.
    """
    # name_or_addr is optional and only names the scratch file. Guarding here
    # rather than inside _resolve, which is entitled to assume a string: without
    # it, omitting the argument -- exactly what "optional" invites -- failed with
    # an AttributeError on None that pointed nowhere near the cause.
    if compiler_profile not in agbenv.compiler_profiles():
        return {"ok": False,
                "error": "unknown compiler_profile %r; choose %s" %
                         (compiler_profile,
                          ", ".join(agbenv.compiler_profiles()))}
    stem = (_resolve(name_or_addr) or {}).get("name") if name_or_addr else None
    if cflags_add or cflags_remove:
        try:
            compiler_profile = agbenv.custom_profile(
                cflags_add or [], cflags_remove or [], base=compiler_profile,
                fn=stem)
        except ValueError as exc:
            return {"ok": False, "error": str(exc)}
    stem = stem or "probe"
    suffix = "" if compiler_profile == "configured" else "." + compiler_profile
    rel = "build/probe/%s%s.c" % (stem, suffix)
    awlib.write_text(os.path.join(awlib.REPO, rel.replace("/", os.sep)), c_code)

    f = agbenv.flags(stem if name_or_addr else None,
                     profile=compiler_profile)
    out_s = "build/probe/%s%s.s" % (stem, suffix)
    rc, so, se = agbenv.run(
        'mkdir -p build/probe\n'
        '%s %s %s | iconv -f UTF-8 -t CP932 | %s %s -o %s'
        % (f["CPP"], f["CPPFLAGS"], rel, f["CC1"], f["CFLAGS"], out_s))
    if rc != 0:
        return {"ok": False,
                "error": "compile failed -- note -Werror is on, so a warning "
                         "is fatal",
                "stderr": (se or so)[-3000:]}

    path = os.path.join(awlib.REPO, out_s.replace("/", os.sep))
    text = "".join(awlib.read_lines(path)) if os.path.exists(path) else ""
    # Strip directives and debug line markers; only instructions matter here.
    body = [ln.rstrip() for ln in text.splitlines()
            if ln.strip() and not ln.strip().startswith((".file", ".loc", ".ident"))]
    return {"ok": True, "asm": "\n".join(body[:400]),
            "compiler_profile": compiler_profile,
            "provisional": compiler_profile != "configured",
            "lines": len(body),
            "note": "Assembly only -- run try_match for a byte-level verdict."}


@mcp.tool()
def verify_split() -> dict:
    """Check the per-function split is lossless and every unit self-contained.

    Two independent gates: units must reconstruct asm/*.s byte-for-byte, and no
    unit may reference a local label it does not define. Run this after any
    change to the splitter, before trusting a build.
    """
    return _run(["tools/verify_split.py"])


@mcp.tool()
def rom_compare() -> dict:
    """Run both mandatory ROM comparisons inside WSL.

    This is the integration gate after promotion. It removes the shared ROM
    and ELF outputs before each route, runs the split/C and upstream-assembly
    builds independently, and reports each real exit status. Both results must
    be successful and print the canonical SHA1 check before a wave may commit.

    The command is deliberately fixed: callers cannot supply a shell fragment,
    target, path, or cleanup scope.
    """
    repo = "/mnt/d/Codes and Projects/awbh-port/aw2bhr"

    def run_route(make_args: str) -> dict:
        command = (
            'cd "' + repo + '" && '
            'rm -f -- aw2bhr.gba aw2bhr.elf && '
            'make ' + make_args + ' 2>&1'
        )
        try:
            proc = subprocess.run(
                ["wsl", "-d", "Ubuntu", "-e", "bash", "-c", command],
                cwd=awlib.REPO, capture_output=True, text=True, timeout=1200,
                stdin=subprocess.DEVNULL)
        except subprocess.TimeoutExpired as exc:
            value = exc.stdout or ""
            if isinstance(value, bytes):
                value = value.decode("utf-8", errors="replace")
            return {
                "ok": False,
                "exit_code": 124,
                "output": value[-8000:],
                "error": "timed out after 1200s",
            }
        output = (proc.stdout or "") + (proc.stderr or "")
        return {
            "ok": proc.returncode == 0,
            "exit_code": proc.returncode,
            "output": output[-8000:],
        }

    split = run_route("SPLIT=1 compare")
    assembly = run_route("compare")
    expected = "aw2bhr.gba: OK"
    split["sha1_ok"] = expected in split["output"]
    assembly["sha1_ok"] = expected in assembly["output"]
    return {
        "ok": (split["ok"] and split["sha1_ok"] and
               assembly["ok"] and assembly["sha1_ok"]),
        "canonical_sha1": "14dd0b22c894865867aff89e8116b2dffae25605",
        "split": split,
        "assembly": assembly,
    }


@mcp.tool()
def reindex() -> dict:
    """Rebuild data/functions.json and data/callgraph.json from asm/*.s.

    Run after decompiling functions so the work queue reflects reality.
    """
    first = _run(["tools/index_functions.py"])
    if not first["ok"]:
        return {"step": "index_functions", **first}
    second = _run(["tools/callgraph.py"])
    _cache.clear()
    return {
        "step": "complete" if second["ok"] else "callgraph",
        "index_stdout": first["stdout"],
        **second,
    }


if __name__ == "__main__":
    mcp.run()
