"""Run the agbcc toolchain from Windows, with the build flags taken from the Makefile.

Two problems this exists to solve.

The toolchain lives in WSL while the tools are Windows Python, so every compile
has to cross over. Doing that through a shell means quoting the repo path --
which contains a space -- correctly every time, and that has already broken one
vendored script (agbcc's install.sh, whose `[ -d "$1" ]` guard is quoted so it
passes, after which every unquoted `cp` fails). Passing the script as a single
argv element to subprocess removes the shell from the path entirely, so the
space stops mattering.

The flags are read out of the Makefile rather than copied into a constant. A
candidate compiled with flags that differ from the real build in any way will
not match, and the failure looks exactly like a bad decompilation -- the most
expensive kind of false negative there is. Parsing keeps the two in step.

    from agbenv import compile_c, assemble, run
"""

import json
import os
import re
import shlex
import subprocess
import sys

import awlib

DISTRO = os.environ.get("AW2_WSL_DISTRO", "Ubuntu")
MAKEFILE = os.path.join(awlib.REPO, "Makefile")


def wsl_path(win_path):
    r"""Windows path to its WSL mount point: C:\foo\bar -> /mnt/c/foo/bar"""
    p = os.path.abspath(win_path)
    drive, rest = os.path.splitdrive(p)
    if not drive:
        return p.replace("\\", "/")
    return "/mnt/" + drive[0].lower() + rest.replace("\\", "/")


REPO_POSIX = wsl_path(awlib.REPO) if os.name == "nt" else awlib.REPO


def run(script, timeout=900, cwd=None):
    """Run a bash script with the repo as the working directory.

    The script is handed to bash as one argument, so nothing in it is re-parsed
    by an intermediate shell.
    """
    root = cwd or REPO_POSIX
    full = "cd %s || exit 1\n%s" % (shlex.quote(root), script)
    argv = (["wsl", "-d", DISTRO, "-e", "bash", "-c", full]
            if os.name == "nt" else ["bash", "-c", full])
    # stdin must be detached. When this runs underneath the MCP server the
    # process's stdin is the JSON-RPC pipe, capture_output redirects only
    # stdout and stderr, and a child inheriting that pipe hangs indefinitely --
    # which looks like a slow compile rather than a deadlock.
    proc = subprocess.run(argv, capture_output=True, text=True,
                          timeout=timeout, errors="replace",
                          stdin=subprocess.DEVNULL)
    return proc.returncode, proc.stdout, proc.stderr


def stream(script, log_path=None, cwd=None):
    """Like run(), but echo output as it arrives instead of at the end.

    run() buffers, which is right for a compile that takes a second and wrong
    for anything long: a search that prints a live progress line looks exactly
    like a hung process for as long as it runs. Returns (returncode, text).
    """
    root = cwd or REPO_POSIX
    full = "cd %s || exit 1\n%s" % (shlex.quote(root), script)
    argv = (["wsl", "-d", DISTRO, "-e", "bash", "-c", full]
            if os.name == "nt" else ["bash", "-c", full])
    # Binary, and read1() rather than iterating lines: the permuter's progress
    # counter is redrawn with a carriage return and never emits a newline, so
    # line-buffered reading would show nothing until the run ended -- the exact
    # problem this function exists to avoid.
    proc = subprocess.Popen(argv, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
    chunks = []
    log = open(log_path, "w", encoding="utf-8") if log_path else None
    try:
        while True:
            chunk = proc.stdout.read1(4096)
            if not chunk:
                break
            text = chunk.decode("utf-8", errors="replace")
            chunks.append(text)
            if log:
                log.write(text)
                log.flush()
            sys.stdout.write(text)
            sys.stdout.flush()
    finally:
        if log:
            log.close()
        proc.stdout.close()
    return proc.wait(), "".join(chunks)


_PROGRESS_RE = re.compile(r'^iteration \d+')
_ANSI_RE = re.compile(r'\x1b\[[0-9;]*[A-Za-z]')


def stream_compact(script, log_path, echo=False, every=60, cwd=None):
    """Like stream(), but keep the log READABLE and the caller's output quiet.

    The permuter redraws `iteration N, E errors, score = S` with carriage
    returns and backspaces, never a newline: about 400 KB a minute, all of it
    one "line" to `tail`. Here every CR/LF-terminated segment is cleaned of
    ANSI colour and backspaces; progress segments are kept only as a snapshot
    every `every` seconds plus the final one, and every other message (new
    best score, errors, the permuter's own report) is kept whole. A 15-minute
    run's log drops from ~6 MB to a few KB and `tail -25` means something.

    echo=True also prints the kept lines. Returns (returncode, kept text).
    """
    import time
    root = cwd or REPO_POSIX
    full = "cd %s || exit 1\n%s" % (shlex.quote(root), script)
    argv = (["wsl", "-d", DISTRO, "-e", "bash", "-c", full]
            if os.name == "nt" else ["bash", "-c", full])
    proc = subprocess.Popen(argv, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
    kept, latest = [], None
    start = last = time.time()
    log = open(log_path, "w", encoding="utf-8") if log_path else None

    def emit(line):
        kept.append(line)
        if log:
            log.write(line + "\n")
            log.flush()
        if echo:
            sys.stdout.write(line + "\n")
            sys.stdout.flush()

    def feed(seg):
        nonlocal latest, last
        seg = _ANSI_RE.sub("", seg).replace("\b", "").strip()
        if not seg:
            return
        if _PROGRESS_RE.match(seg):
            latest = seg
            now = time.time()
            if now - last >= every:
                emit("[%5ds] %s" % (now - start, seg))
                last = now
            return
        emit(seg)

    buf = ""
    try:
        while True:
            chunk = proc.stdout.read1(4096)
            if not chunk:
                break
            buf += chunk.decode("utf-8", errors="replace")
            parts = re.split(r'[\r\n]', buf)
            buf = parts.pop()
            for seg in parts:
                feed(seg)
        feed(buf)
        if latest:
            emit("[%5ds] %s  (final)" % (time.time() - start, latest))
    finally:
        if log:
            log.close()
        proc.stdout.close()
    return proc.wait(), "\n".join(kept)


_VAR_RE_CACHE = {}


def makefile_var(name):
    """The value of a `NAME := ...` assignment, with $(...) refs expanded.

    Only the handful of variables the compile pipeline needs are resolvable;
    anything still unexpanded is left alone and will surface as a build error
    rather than being silently dropped.
    """
    if not _VAR_RE_CACHE:
        with open(MAKEFILE, encoding="utf-8", errors="replace") as fh:
            for ln in fh:
                m = re.match(r'^\s*(?:export\s+)?(\w+)\s*[:?]?=\s*(.*?)\s*$', ln)
                if m and m.group(1) not in _VAR_RE_CACHE:
                    _VAR_RE_CACHE[m.group(1)] = m.group(2)
    known = {
        "AGBCC_HOME": "tools/agbcc",
        "PREFIX": "arm-none-eabi-",
        "EXE": "",
        "BUILD_DIR": "build",
        "SRC_DIR": "src",
        "ASM_DIR": "asm",
    }
    value = known.get(name, _VAR_RE_CACHE.get(name, ""))
    for _ in range(5):
        expanded = re.sub(r'\$\((\w+)\)',
                          lambda m: known.get(m.group(1),
                                              _VAR_RE_CACHE.get(m.group(1), m.group(0))),
                          value)
        if expanded == value:
            break
        value = expanded
    return value.strip()


_OVERRIDES = None


# Temporary compiler configurations for investigating a draft. `configured`
# is the only profile that consults compiler-overrides.json and is therefore
# the only one suitable for canonical verification and promotion.
COMPILER_PROFILES = {
    "configured": None,
    "default": {},
    "no-force": {"cflags_remove": ["-fforce-addr"]},
    "o1": {
        "cflags_remove": ["-O2"],
        "cflags_add": ["-O1"],
    },
    "o1-no-force": {
        "cflags_remove": ["-O2", "-fforce-addr"],
        "cflags_add": ["-O1"],
    },
    "old-agbcc": {
        "cc1": "old_agbcc",
        "cflags_remove": ["-fprologue-bugfix"],
    },
    "old-agbcc-no-force": {
        "cc1": "old_agbcc",
        "cflags_remove": ["-fprologue-bugfix", "-fforce-addr"],
    },
}


def compiler_overrides():
    """data/compiler-overrides.json, or {} if it is absent."""
    global _OVERRIDES
    if _OVERRIDES is None:
        path = os.path.join(awlib.REPO, "data", "compiler-overrides.json")
        try:
            with open(path, encoding="utf-8") as fh:
                _OVERRIDES = json.load(fh).get("functions", {})
        except (IOError, OSError, ValueError):
            _OVERRIDES = {}
    return _OVERRIDES


def compiler_profiles():
    return tuple(COMPILER_PROFILES)


def profile_override(profile):
    """Return a copy of a temporary profile's override-shaped definition."""
    if profile not in COMPILER_PROFILES:
        raise ValueError("unknown compiler profile %r (choose %s)" %
                         (profile, ", ".join(compiler_profiles())))
    spec = COMPILER_PROFILES[profile]
    if spec is None:
        return None
    return {k: list(v) if isinstance(v, list) else v
            for k, v in spec.items()}


_FLAG_RE = re.compile(r'^-[A-Za-z0-9_=.+-]+$')


def custom_profile(add=(), remove=(), base="configured", fn=None):
    """Register the one-off profile "custom" and return its name.

    Starts from `base` (for "configured", `fn`'s entry in
    compiler-overrides.json, if any), then removes and adds single cflags.
    The named profiles change several flags at once (o1 differs from -O2 in
    about ten), so they cannot say which flag matters; `-O2 -fno-gcse` alone
    is what matched sub_0805D438. Like every non-configured profile, a match
    under it is provisional.
    """
    for opt in list(add) + list(remove):
        if not _FLAG_RE.match(opt):
            raise ValueError("not a compiler flag: %r" % opt)
    if base == "configured":
        spec = dict(compiler_overrides().get(fn) or {}) if fn else {}
    else:
        spec = profile_override(base) or {}
    out = {}
    if spec.get("cc1"):
        out["cc1"] = spec["cc1"]
    out["cflags_remove"] = list(dict.fromkeys(
        [x for x in spec.get("cflags_remove", []) if x not in add] + list(remove)))
    out["cflags_add"] = list(dict.fromkeys(
        [x for x in spec.get("cflags_add", []) if x not in remove] + list(add)))
    COMPILER_PROFILES["custom"] = out
    return "custom"


def _apply_override(f, ov):
    if not ov:
        return
    if ov.get("cc1"):
        # Forward slashes deliberately: this string is handed to a POSIX shell,
        # and os.path.join would emit backslashes on Windows that bash discards.
        f["CC1"] = "%s/bin/%s" % (makefile_var("AGBCC_HOME") or "tools/agbcc",
                                    ov["cc1"])
    remove = set(ov.get("cflags_remove", ()))
    if remove:
        f["CFLAGS"] = " ".join(x for x in f["CFLAGS"].split()
                                if x not in remove)
    for opt in ov.get("cflags_add", ()):
        if opt not in f["CFLAGS"].split():
            f["CFLAGS"] += " " + opt


def flags(fn=None, profile="configured"):
    """Build flags, with `fn`'s per-function override applied if it has one.

    Parts of the ROM were not built with the default toolchain -- the sound and
    flash libraries came prebuilt out of the SDK. Passing the function name here
    is what keeps trymatch.py and compile_probe judging a candidate the way the
    ROM was actually built. Without it they contradict the Makefile, report an
    already-solved function as a failure, and every later wave re-attempts it.
    See data/compiler-overrides.json. A non-configured `profile` deliberately
    ignores that file and applies one temporary experiment to the Makefile
    defaults; callers must not treat such a result as promotion-ready.
    """
    f = {
        "CPPFLAGS": makefile_var("CPPFLAGS"),
        "CFLAGS": makefile_var("CFLAGS"),
        "ASFLAGS": makefile_var("ASFLAGS"),
        "CPP": makefile_var("CPP") or "arm-none-eabi-cpp",
        "AS": makefile_var("AS") or "arm-none-eabi-as",
        "STRIP": makefile_var("STRIP") or "arm-none-eabi-strip",
        "CC1": makefile_var("CC1") or "tools/agbcc/bin/agbcc",
    }
    if profile not in COMPILER_PROFILES:
        raise ValueError("unknown compiler profile %r (choose %s)" %
                         (profile, ", ".join(compiler_profiles())))
    ov = ((compiler_overrides().get(fn) if fn else None)
          if profile == "configured" else COMPILER_PROFILES[profile])
    _apply_override(f, ov)
    return f


def compile_c(src, out_o, out_s=None, extra_cflags="", fn=None,
              profile="configured"):
    """Compile one C file exactly as the Makefile's `$(BUILD_DIR)/%.o: %.c` does.

    The `.text/.align` line appended after agbcc and the `.gcc2_compiled.` strip
    are part of that recipe, not incidental -- omitting either changes the
    object and so changes whether a candidate is judged to match.
    """
    f = flags(fn, profile=profile)
    out_s = out_s or (os.path.splitext(out_o)[0] + ".s")
    script = (
        'set -e\n'
        'mkdir -p "$(dirname {o})" "$(dirname {s})"\n'
        '{cpp} {cppflags} {src} | iconv -f UTF-8 -t CP932 | '
        '{cc1} {cflags} {extra} -o {s}\n'
        'printf ".text\\n\\t.align\\t2, 0\\n" >> {s}\n'
        '{as_} {asflags} {s} -o {o}\n'
        '{strip} -N .gcc2_compiled. {o}\n'
    ).format(
        cpp=f["CPP"], cppflags=f["CPPFLAGS"], src=shlex.quote(src),
        cc1=f["CC1"], cflags=f["CFLAGS"], extra=extra_cflags,
        s=shlex.quote(out_s), as_=f["AS"], asflags=f["ASFLAGS"],
        o=shlex.quote(out_o), strip=f["STRIP"])
    return run(script)


def uninitialized_reads(src, fn=None, profile="configured"):
    """Names of locals agbcc says `src` might read before setting them.

    The permuter's commonest wrong-C form reads a local before any assignment
    reaches it; in wave 93 three of five kept "improvements" were that, and
    they scored well because the undefined read reuses a register the ROM
    also reuses. gcc's own flow analysis names them. The build's -Werror turns
    the warning into a failed compile, so this compiles a throwaway object
    under build/uninit/ and reads only the messages; it cannot change a verdict.
    gcc can also warn about a local that is set on every path it cannot prove,
    so compare against the draft's own list before calling a name wrong.
    """
    safe = re.sub(r'[^A-Za-z0-9_.-]', '_', os.path.relpath(src, awlib.REPO))
    out = "build/uninit/%s.o" % safe
    rel = os.path.relpath(src, awlib.REPO).replace(os.sep, "/")
    rc, so, se = compile_c(rel, out, extra_cflags="-Wuninitialized",
                           fn=fn, profile=profile)
    names = re.findall(r"`(\w+)' might be used uninitialized", so + se)
    return sorted(set(names))


def assemble(src_s, out_o):
    """Assemble one .s exactly as the Makefile's asm rule does."""
    f = flags()
    script = ('set -e\nmkdir -p "$(dirname {o})"\n{as_} {asflags} {s} -o {o}\n'
              ).format(as_=f["AS"], asflags=f["ASFLAGS"],
                       s=shlex.quote(src_s), o=shlex.quote(out_o))
    return run(script)


def objdump(obj, extra="-d -r -z"):
    f = flags()
    return run("%sobjdump %s %s" % (makefile_var("PREFIX") or "arm-none-eabi-",
                                    extra, shlex.quote(obj)))


def self_test():
    """Prove the pipeline here is the same one the Makefile uses.

    Compiles src/proc.c through this module and compares the object with the
    one `make` produced. Byte equality is the only acceptable result: if these
    differ, every match verdict from this harness is untrustworthy, and the
    difference would otherwise show up as an unexplainable failure to match.
    """
    ref = os.path.join(awlib.REPO, "build", "src", "proc.o")
    if not os.path.exists(ref):
        print("error: build/src/proc.o missing -- run `make` first")
        return 1

    out = "build/selftest/proc.o"
    rc, so, se = compile_c("src/proc.c", out)
    if rc != 0:
        print("FAIL compile returned %d\n%s\n%s" % (rc, so[-2000:], se[-2000:]))
        return 1

    got = os.path.join(awlib.REPO, "build", "selftest", "proc.o")
    a, b = open(ref, "rb").read(), open(got, "rb").read()
    if a != b:
        first = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]),
                     min(len(a), len(b)))
        print("FAIL objects differ at byte %d (make=%d bytes, harness=%d bytes)"
              % (first, len(a), len(b)))
        print("     the harness is not reproducing the real build's flags")
        return 1

    print("PASS  harness reproduces build/src/proc.o byte-for-byte (%,d bytes)"
          .replace("%,d", "{:,}").format(len(a)))
    print("      compile flags match the Makefile, so match verdicts are sound")
    return 0


if __name__ == "__main__":
    if "--flags" in sys.argv:
        for k, v in flags().items():
            print("%-9s %s" % (k, v))
        sys.exit(0)
    sys.exit(self_test())
