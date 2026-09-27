#!/usr/bin/env python3
"""Generate progress/index.html -- a memory map of decompilation progress.

Every function in the ROM's code region is one rectangle, sized by its byte
count and placed in the address band it lives in, so the picture is literally a
map of the cartridge rather than a bar chart.

Six exclusive states keep the linker map distinct from decomp progress:

  promoted     decompiled into src/decomp and proven by `make compare`
  upstream     pre-existing C in src/proc.c or src/title-screen.c
  active       unmatched assembly still in the live queue
  parked       compiler output with a settled near-miss draft
  asm-resident hand-written assembly/padding that C should not reproduce
  identified   active assembly named from the Fire Emblem decomps

The headline percentage uses only data/functions.json, so upstream C cannot
inflate decomp progress. The map still includes it because omitting real code
would make the address visualization incomplete.

Promoted addresses and sizes come from the function index. The linker map adds
the pre-existing C functions that were never part of the assembly index; without
it the address view would silently omit real code.

Usage:
    python tools/progress_map.py [--out progress/index.html]
"""

import argparse
import html
import json
import os
import re
import sys
from collections import defaultdict

import awlib

OUT = os.path.join("progress", "index.html")
BAND = 32 * 1024                      # address band per treemap module
WIDTH, HEIGHT = 1240, 780

CLS = {
    "promoted": "m",
    "upstream": "c",
    "active": "u",
    "parked": "k",
    "asm-resident": "r",
    "identified": "p",
}
STATUS_ORDER = ("promoted", "upstream", "active", "parked",
                "asm-resident", "identified")


# ---------------------------------------------------------------- data

def load_asm_functions():
    path = os.path.join(awlib.DATA_DIR, "functions.json")
    if not os.path.exists(path):
        print("error: data/functions.json missing -- run tools/index_functions.py")
        return None
    with open(path, encoding="utf-8") as fh:
        return json.load(fh)


def load_fe_names():
    path = os.path.join(awlib.DATA_DIR, "fe_matches.json")
    if not os.path.exists(path):
        return {}
    with open(path, encoding="utf-8") as fh:
        return {h["name"]: h["fe_name"] for h in json.load(fh)["matches"]}


def load_status_names(filename):
    path = os.path.join(awlib.DATA_DIR, filename)
    if not os.path.exists(path):
        return set()
    with open(path, encoding="utf-8") as fh:
        payload = json.load(fh).get("functions", {})
    return set(payload if isinstance(payload, dict) else payload)


MAP_OBJ_RE = re.compile(r'^\s\.text\s+0x([0-9a-f]{8})\s+0x([0-9a-f]+)\s+(\S+)')
MAP_SYM_RE = re.compile(r'^\s+0x([0-9a-f]{8})\s{2,}(\S+)\s*$')


def c_objects():
    """Objects the linker sees for each src/*.c -- the decompiled units.

    The link runs from build/, so the map names them `src/proc.o`, not
    `build/src/proc.o`. Deriving the set from the C sources rather than
    pattern-matching a path also keeps src/crt0.o and src/rom-header.o out,
    which are hand-written assembly and were never decompiled.
    """
    src = os.path.join(awlib.REPO, "src")
    if not os.path.isdir(src):
        return set()
    out = set()
    for dirpath, _, filenames in os.walk(src):
        rel = os.path.relpath(dirpath, awlib.REPO).replace("\\", "/")
        for e in filenames:
            if e.endswith(".c"):
                out.add("%s/%s.o" % (rel, os.path.splitext(e)[0]))
    return out


def load_upstream_from_map(asm):
    """Pre-existing C functions recovered from the linker map.

    Assembly-index functions are skipped by address as well as name. The
    source definition may have a human-readable name while the assembly index
    still uses sub_XXXXXXXX; address matching keeps renamed promoted functions
    out of the upstream bucket. This leaves pre-existing C functions such as
    those in src/proc.c and src/title-screen.c.
    """
    path = os.path.join(awlib.REPO, "aw2bhr.map")
    if not os.path.exists(path):
        print("note: aw2bhr.map missing -- run `make` for completed functions "
              "to appear; showing assembly only")
        return []

    known_names = {r["name"] for r in asm}
    promoted_path = os.path.join(awlib.DATA_DIR, "promoted.json")
    if os.path.exists(promoted_path):
        with open(promoted_path, encoding="utf-8") as fh:
            for unit in json.load(fh):
                known_names.update(unit.get("functions", []))
    known_addrs = {r["addr"] for r in asm}
    wanted = c_objects()
    out, cur_obj, cur_end = [], None, None
    with open(path, encoding="utf-8", errors="replace") as fh:
        for ln in fh:
            m = MAP_OBJ_RE.match(ln)
            if m:
                cur_obj = m.group(3).replace("\\", "/")
                cur_end = int(m.group(1), 16) + int(m.group(2), 16)
                continue
            m = MAP_SYM_RE.match(ln)
            if not m or cur_obj not in wanted:
                continue
            name, addr = m.group(2), int(m.group(1), 16)
            if name in known_names or addr in known_addrs or name.startswith("."):
                continue
            out.append({"name": name, "addr": addr, "obj": cur_obj,
                        "end": cur_end})

    out.sort(key=lambda r: r["addr"])
    for i, r in enumerate(out):
        nxt = out[i + 1] if i + 1 < len(out) else None
        # Bound by the object's own section end: the last function in a unit
        # would otherwise absorb the gap to whatever was linked after it.
        limit = r["end"]
        if nxt is not None and nxt["obj"] == r["obj"]:
            limit = min(limit, nxt["addr"])
        r["size"] = max(2, limit - r["addr"])
    return out


def collect():
    asm = load_asm_functions()
    if asm is None:
        return None
    fe = load_fe_names()
    parked = load_status_names("parked.json")
    resident = load_status_names("asm-resident.json")
    items = []
    for r in asm:
        # Promoted functions remain in asm/ as reference, so presence there says
        # nothing; the index's status does. Taking it from the index also keeps
        # the page honest regardless of which build wrote aw2bhr.map last -- the
        # upstream build deliberately excludes src/decomp.
        fe_note = fe.get(r["name"])
        if r.get("status") == "matched":
            status, note = "promoted", "decompiled from assembly"
        elif r["name"] in resident:
            status, note = "asm-resident", "not compiler-generated"
            if fe_note:
                note += "; FE shape: " + fe_note
        elif r["name"] in parked:
            status, note = "parked", "settled compiler-output near-miss"
            if fe_note:
                note += "; FE shape: " + fe_note
        elif r["name"] in fe:
            status, note = "identified", fe[r["name"]]
        else:
            status, note = "active", "unmatched assembly in the live queue"
        items.append({
            "name": r["name"], "addr": r["addr"], "size": max(2, r["size"]),
            "status": status, "note": note,
        })
    for r in load_upstream_from_map(asm):
        items.append({"name": r["name"], "addr": r["addr"],
                      "size": max(2, r["size"]), "status": "upstream",
                      "note": "pre-existing C: " + r["obj"]})
    items.sort(key=lambda r: r["addr"])
    return items


# ---------------------------------------------------------------- treemap

def _worst(row, side):
    total = sum(row)
    if total <= 0 or side <= 0:
        return float("inf")
    return max(side * side * max(row) / (total * total),
               total * total / (side * side * min(row)))


def _layout_row(row, x, y, dx, dy):
    covered = sum(row)
    rects = []
    if dx >= dy:
        w = covered / dy if dy else 0
        cy = y
        for s in row:
            h = s / w if w else 0
            rects.append((x, cy, w, h))
            cy += h
        return rects, x + w, y, dx - w, dy
    h = covered / dx if dx else 0
    cx = x
    for s in row:
        w = s / h if h else 0
        rects.append((cx, y, w, h))
        cx += w
    return rects, x, y + h, dx, dy - h


def squarify(sizes, x, y, dx, dy):
    """Squarified treemap over `sizes` (descending), scaled to fill dx*dy."""
    total = sum(sizes)
    if total <= 0 or dx <= 0 or dy <= 0:
        return []
    scale = dx * dy / total
    remaining = [s * scale for s in sizes]
    out = []
    while remaining and dx > 1e-9 and dy > 1e-9:
        side = min(dx, dy)
        row, i = [remaining[0]], 1
        while i < len(remaining):
            if _worst(row + [remaining[i]], side) <= _worst(row, side):
                row.append(remaining[i])
                i += 1
            else:
                break
        rects, x, y, dx, dy = _layout_row(row, x, y, dx, dy)
        out.extend(rects)
        remaining = remaining[i:]
    return out


# ---------------------------------------------------------------- render

def band_key(addr):
    base = addr - (addr % BAND)
    return base


def render_svg(items):
    bands = defaultdict(list)
    for it in items:
        bands[band_key(it["addr"])].append(it)

    order = sorted(bands, key=lambda b: -sum(i["size"] for i in bands[b]))
    band_sizes = [sum(i["size"] for i in bands[b]) for b in order]
    outer = squarify(band_sizes, 0, 0, WIDTH, HEIGHT)

    parts = []
    for base, (bx, by, bw, bh) in zip(order, outer):
        group = sorted(bands[base], key=lambda i: -i["size"])
        done = sum(i["size"] for i in group
                   if i["status"] in ("promoted", "upstream"))
        total = sum(i["size"] for i in group)
        pct = done / total * 100 if total else 0

        parts.append('<g class="mod">')
        parts.append('<rect class="mframe" x="%.1f" y="%.1f" width="%.1f" '
                     'height="%.1f"/>' % (bx, by, max(0, bw), max(0, bh)))
        if bw > 90 and bh > 16:
            parts.append('<text class="mlabel" x="%.1f" y="%.1f">0x%08X'
                         '<tspan class="mpct"> %.1f%%</tspan></text>'
                         % (bx + 3, by + 11, base, pct))

        pad_top = 14 if (bw > 90 and bh > 16) else 2
        inner = squarify([i["size"] for i in group],
                         bx + 1, by + pad_top, max(0, bw - 2),
                         max(0, bh - pad_top - 1))
        for it, (x, y, w, h) in zip(group, inner):
            parts.append(
                '<rect class="c %s" x="%.1f" y="%.1f" width="%.1f" height="%.1f"'
                ' data-n="%s" data-a="%08X" data-s="%d" data-t="%s" data-p="%s"/>'
                % (CLS[it["status"]], x, y, max(0.4, w), max(0.4, h),
                   html.escape(it["name"], quote=True), it["addr"], it["size"],
                   it["status"], html.escape(it["note"], quote=True)))
        parts.append('</g>')
    return "".join(parts), order, bands


def tile(kind, n, nbytes, tot_n, tot_b, blurb):
    return (
        '<div class="tile %s"><div class="tile-label">%s</div>'
        '<div class="tile-num">%s<span class="tile-unit"> functions</span></div>'
        '<div class="tile-sub">%.2f%% of functions &middot; %s bytes '
        '(%.2f%% of code)<br>%s</div></div>'
        % (kind, kind, "{:,}".format(n),
           n / tot_n * 100 if tot_n else 0, "{:,}".format(nbytes),
           nbytes / tot_b * 100 if tot_b else 0, blurb))


CSS = """
:root {
  color-scheme: light dark;
  --surface:#fcfcfb; --panel:#f2f1ee; --ink:#0b0b0b; --ink-2:#52514e;
  --frame:#d8d7d2; --cell-active:#e4e3df;
}
@media (prefers-color-scheme: dark) { :root:where(:not([data-theme="light"])) {
  --surface:#1a1a19; --panel:#232322; --ink:#fff; --ink-2:#c3c2b7;
  --frame:#3d3d3b; --cell-active:#33332f;
} }
:root[data-theme="dark"] {
  --surface:#1a1a19; --panel:#232322; --ink:#fff; --ink-2:#c3c2b7;
  --frame:#3d3d3b; --cell-active:#33332f;
}
:root[data-theme="light"] {
  --surface:#fcfcfb; --panel:#f2f1ee; --ink:#0b0b0b; --ink-2:#52514e;
  --frame:#d8d7d2; --cell-active:#e4e3df;
}
body { background:var(--surface); color:var(--ink);
  font:14px/1.5 ui-sans-serif, system-ui, sans-serif;
  margin:0; padding:24px clamp(12px,4vw,48px); }
h1 { font-size:20px; margin:0 0 2px; }
.sub { color:var(--ink-2); margin:0 0 18px; font-size:13px; max-width:78ch; }
.tiles { display:flex; gap:12px; flex-wrap:wrap; margin:0 0 18px; }
.tile { background:var(--panel); border:1px solid var(--frame);
  border-radius:8px; padding:10px 16px 12px; min-width:210px;
  border-top:3px solid var(--frame); }
.tile.promoted { border-top-color:#0ca30c; }
.tile.upstream { border-top-color:#3182ce; }
.tile.active { border-top-color:var(--frame); }
.tile.parked { border-top-color:#ed8936; }
.tile.asm-resident { border-top-color:#9f7aea; }
.tile.identified { border-top-color:#fab219; }
.tile-label { font-size:12px; text-transform:uppercase; letter-spacing:.06em;
  color:var(--ink-2); }
.tile-num { font-size:26px; font-weight:650; font-variant-numeric:tabular-nums; }
.tile-unit { font-size:13px; font-weight:400; color:var(--ink-2); }
.tile-sub { font-size:12px; color:var(--ink-2); }
.legend { display:flex; gap:18px; align-items:center; font-size:12.5px;
  color:var(--ink-2); margin:0 0 8px; flex-wrap:wrap; }
.sw { display:inline-block; width:11px; height:11px; border-radius:3px;
  margin-right:6px; vertical-align:-1px; }
.map-wrap { overflow-x:auto; }
svg { display:block; border-radius:6px; }
.mframe { fill:none; stroke:var(--frame); stroke-width:1; }
.mlabel { font:600 10.5px ui-monospace, monospace; fill:var(--ink); }
.mpct { font-weight:400; fill:var(--ink-2); }
.c { shape-rendering:crispEdges; }
.c.u { fill:var(--cell-active); }
.c.m { fill:#0ca30c; }
.c.c { fill:#3182ce; }
.c.k { fill:#ed8936; }
.c.r { fill:#9f7aea; }
.c.p { fill:#fab219; }
.c:hover { stroke:var(--ink); stroke-width:1; }
#tip { position:fixed; pointer-events:none; background:var(--panel);
  color:var(--ink); border:1px solid var(--frame); border-radius:6px;
  padding:6px 9px; font-size:12px; display:none; z-index:9; max-width:340px;
  box-shadow:0 4px 14px rgb(0 0 0 / .25); }
#tip .mono { font-family:ui-monospace, monospace; }
table { border-collapse:collapse; margin-top:22px; font-size:13px; }
th, td { padding:4px 12px; text-align:left; border-bottom:1px solid var(--frame); }
th { color:var(--ink-2); font-weight:600; font-size:12px; }
td.num { text-align:right; font-variant-numeric:tabular-nums; }
td.mono { font-family:ui-monospace, monospace; font-size:12px; }
.foot { color:var(--ink-2); font-size:12px; margin-top:20px; max-width:78ch; }
"""

SCRIPT = """
(function () {
  var tip = document.getElementById("tip");
  var svg = document.querySelector("svg");
  if (!svg) return;
  svg.addEventListener("mousemove", function (e) {
    var t = e.target;
    if (!t.classList || !t.classList.contains("c")) { tip.style.display="none"; return; }
    tip.innerHTML = "<span class='mono'>" + t.dataset.n + "</span><br>0x" +
      t.dataset.a + " &middot; " + t.dataset.s + " bytes &middot; " + t.dataset.t +
      (t.dataset.p ? "<br>" + t.dataset.p : "");
    tip.style.display = "block";
    var x = e.clientX + 14, y = e.clientY + 14;
    var r = tip.getBoundingClientRect();
    if (x + r.width  > innerWidth  - 8) x = e.clientX - r.width  - 10;
    if (y + r.height > innerHeight - 8) y = e.clientY - r.height - 10;
    tip.style.left = x + "px"; tip.style.top = y + "px";
  });
  svg.addEventListener("mouseleave", function(){ tip.style.display="none"; });
})();
"""


def build(out_rel):
    items = collect()
    if items is None:
        return 1

    tot_n = len(items)
    tot_b = sum(i["size"] for i in items)
    by = defaultdict(lambda: [0, 0])
    for i in items:
        by[i["status"]][0] += 1
        by[i["status"]][1] += i["size"]

    tracked_n = sum(by[k][0] for k in STATUS_ORDER if k != "upstream")
    tracked_b = sum(by[k][1] for k in STATUS_ORDER if k != "upstream")
    promoted_n, promoted_b = by["promoted"]
    decomp_fn_pct = promoted_n / tracked_n * 100 if tracked_n else 0
    decomp_byte_pct = promoted_b / tracked_b * 100 if tracked_b else 0

    svg_body, order, bands = render_svg(items)

    rows = []
    for base in sorted(bands):
        group = bands[base]
        n = len(group)
        b = sum(i["size"] for i in group)
        m = sum(1 for i in group if i["status"] == "promoted")
        mb = sum(i["size"] for i in group if i["status"] == "promoted")
        upstream = sum(1 for i in group if i["status"] == "upstream")
        parked = sum(1 for i in group if i["status"] == "parked")
        resident = sum(1 for i in group if i["status"] == "asm-resident")
        ident = sum(1 for i in group if i["status"] == "identified")
        rows.append(
            "<tr><td class='mono'>0x%08X</td><td class='num'>%s</td>"
            "<td class='num'>%s</td><td class='num'>%s</td>"
            "<td class='num'>%.2f%%</td><td class='num'>%s</td>"
            "<td class='num'>%s</td><td class='num'>%s</td>"
            "<td class='num'>%s</td></tr>"
            % (base, "{:,}".format(n), "{:,}".format(b), "{:,}".format(m),
               mb / b * 100 if b else 0,
               "{:,}".format(upstream) if upstream else "",
               "{:,}".format(parked) if parked else "",
               "{:,}".format(resident) if resident else "",
               "{:,}".format(ident) if ident else ""))

    doc = """<!doctype html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Advance Wars 2 &middot; decomp progress</title>
<style>%s</style></head><body>

<h1>Advance Wars 2: Black Hole Rising &mdash; decompilation progress</h1>
<p class="sub"><strong>Decomp progress: %s / %s functions (%.2f%%) &middot;
%s / %s indexed bytes (%.2f%%).</strong><br>%s functions &middot; %s mapped bytes
&middot; %d address bands. The larger map also includes pre-existing upstream C;
band labels show all C-built bytes, while the headline never counts upstream C
as decompilation progress.</p>

<div class="tiles">%s%s%s%s%s%s</div>

<div class="legend">
  <span><span class="sw" style="background:#0ca30c"></span>promoted decomp C</span>
  <span><span class="sw" style="background:#3182ce"></span>pre-existing upstream C</span>
  <span><span class="sw" style="background:var(--cell-active);border:1px solid var(--frame)"></span>active unmatched assembly</span>
  <span><span class="sw" style="background:#ed8936"></span>parked near-miss</span>
  <span><span class="sw" style="background:#9f7aea"></span>asm-resident</span>
  <span><span class="sw" style="background:#fab219"></span>FE-identified active assembly</span>
</div>

<div class="map-wrap">
<svg width="%d" height="%d" viewBox="0 0 %d %d" role="img"
     aria-label="Treemap of decompilation progress by address">%s</svg>
</div>
<div id="tip"></div>

<table>
<thead><tr><th>address band</th><th>funcs</th><th>bytes</th><th>promoted</th>
<th>%% bytes promoted</th><th>upstream C</th><th>parked</th>
<th>asm-resident</th><th>identified</th></tr></thead>
<tbody>%s</tbody></table>

<p class="foot">Generated by <span class="mono">tools/progress_map.py</span> &mdash;
do not edit by hand. Regenerate after
<span class="mono">tools/index_functions.py</span> and a build. Decomp status
comes from <span class="mono">data/functions.json</span>; pre-existing upstream
C addresses come from <span class="mono">aw2bhr.map</span>.
The ROM is %s bytes; only the %s-byte code region is mapped here. The remaining
data is still <span class="mono">.incbin</span> and is a separate problem.</p>

<script>%s</script>
</body></html>
""" % (CSS,
       "{:,}".format(promoted_n), "{:,}".format(tracked_n), decomp_fn_pct,
       "{:,}".format(promoted_b), "{:,}".format(tracked_b), decomp_byte_pct,
       "{:,}".format(tot_n), "{:,}".format(tot_b), len(bands),
       tile("promoted", by["promoted"][0], by["promoted"][1], tot_n, tot_b,
            "byte-for-byte decomp C; primary progress numerator"),
       tile("upstream", by["upstream"][0], by["upstream"][1], tot_n, tot_b,
            "pre-existing C; mapped but excluded from decomp progress"),
       tile("active", by["active"][0], by["active"][1], tot_n, tot_b,
            "unmatched assembly in the live queue"),
       tile("parked", by["parked"][0], by["parked"][1], tot_n, tot_b,
            "compiler output with a settled near-miss draft"),
       tile("asm-resident", by["asm-resident"][0],
            by["asm-resident"][1], tot_n, tot_b,
            "hand-written assembly or upstream-labelled padding"),
       tile("identified", by["identified"][0], by["identified"][1], tot_n, tot_b,
            "FE-named active assembly; not counted as progress"),
       WIDTH, HEIGHT, WIDTH, HEIGHT, svg_body,
       "".join(rows),
       "{:,}".format(8388608), "{:,}".format(tot_b),
       SCRIPT)

    path = os.path.join(awlib.REPO, out_rel.replace("/", os.sep))
    awlib.write_text(path, doc)
    print("wrote %s  (%s functions, %s bands, %s KB)"
          % (out_rel, "{:,}".format(tot_n), len(bands),
             "{:,}".format(len(doc) // 1024)))

    # Mirror to the workspace root, beside rom/ and aw2bhr/, which is where the
    # equivalent page lives in the other decomp projects and so where it gets
    # looked for. Written from the same string in the same run, so the two
    # cannot drift; the in-repo copy is the one under version control and is
    # what gives progress a history.
    mirror = os.path.join(os.path.dirname(awlib.REPO), "progress", "index.html")
    try:
        awlib.write_text(mirror, doc)
        print("mirrored to %s" % mirror)
    except OSError as exc:
        print("note: could not mirror to the workspace root (%s)" % exc)
    print("  decomp      %5s / %s functions  %9s / %s bytes  %.2f%%"
          % ("{:,}".format(promoted_n), "{:,}".format(tracked_n),
             "{:,}".format(promoted_b), "{:,}".format(tracked_b),
             decomp_byte_pct))
    for k in STATUS_ORDER:
        print("  %-11s %5s functions  %9s bytes"
              % (k, "{:,}".format(by[k][0]), "{:,}".format(by[k][1])))
    return 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-o", "--out", default=OUT)
    args = ap.parse_args()
    return build(args.out.replace("\\", "/"))


if __name__ == "__main__":
    sys.exit(main())
