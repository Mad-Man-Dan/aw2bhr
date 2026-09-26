#!/usr/bin/env python3
"""Bounded, sequential permuter sweep over eligible single-function drafts.

The normal tools/permute.py wrapper remains the only scorer/verifier. This
driver merely inventories safe targets, apportions a wall-clock budget, and
keeps a durable per-target log. It always starts from the readable current
draft: a saved best.c may be a semantically invalid permuter mutation.

A run that ends `PERMUTE IMPROVED` (permute.py keeps a measured improvement in
the draft) is recorded as an `improved` event and the next pass starts from
it; any OTHER change to a draft without a verified match still stops the batch.
"""

import argparse
import datetime as dt
import hashlib
import json
import os
import re
import signal
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
WORK = ROOT / "work"


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inventory():
    units = json.loads((ROOT / "build/functions/units.json").read_text())
    promoted = json.loads((ROOT / "data/promoted.json").read_text())
    asm_resident = json.loads((ROOT / "data/asm-resident.json").read_text())
    done = {name for unit in promoted for name in unit.get("functions", [])}
    choices = []
    for unit in units:
        if len(unit["functions"]) != 1:
            continue
        name = unit["functions"][0]
        src = WORK / name / (name + ".c")
        if name in done or name in asm_resident or not src.is_file():
            continue
        if re.search(r"\basm\s*(?:volatile\s*)?\(", src.read_text()):
            continue
        best = WORK / name / "best.json"
        try:
            metrics = json.loads(best.read_text())
        except (OSError, ValueError):
            metrics = {}
        choices.append((name, metrics))
    # Size-exact and close candidates first, while retaining every valid draft.
    choices.sort(key=lambda item: (
        abs(item[1].get("size_delta", 100000)),
        -item[1].get("percent", -1), item[0]))
    return [name for name, _ in choices]


def record(path, event):
    event["utc"] = dt.datetime.now(dt.timezone.utc).isoformat()
    with path.open("a") as out:
        out.write(json.dumps(event, sort_keys=True) + "\n")
        out.flush()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--hours", type=float, default=8)
    ap.add_argument("--deadline-utc",
                    help="absolute ISO-8601 UTC deadline when resuming a batch")
    ap.add_argument("--seconds-per-target", type=int, default=600)
    ap.add_argument("-j", "--threads", type=int, default=4)
    ap.add_argument("--targets", nargs="+",
                    help="run only these eligible functions, in the listed order")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()
    if args.hours <= 0 or args.seconds_per_target < 30 or args.threads < 1:
        ap.error("hours, per-target seconds, and threads must be positive")

    eligible = inventory()
    if args.targets:
        if len(args.targets) != len(set(args.targets)):
            ap.error("--targets contains duplicate functions")
        unknown = sorted(set(args.targets) - set(eligible))
        if unknown:
            ap.error("not eligible for single-function permutation: "
                     + ", ".join(unknown))
        names = args.targets
    else:
        names = eligible
    print("eligible targets:", len(names), flush=True)
    print(" ".join(names), flush=True)
    if args.dry_run:
        return 0
    if not names:
        return 2

    stamp = dt.datetime.now(dt.timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    run_dir = WORK / ("permuter-batch-" + stamp)
    run_dir.mkdir()
    snapshots = run_dir / "starting-drafts"
    snapshots.mkdir()
    source_hashes = {}
    for name in names:
        src = WORK / name / (name + ".c")
        shutil.copyfile(src, snapshots / (name + ".c"))
        source_hashes[name] = digest(src)
    events = run_dir / "events.jsonl"
    now = dt.datetime.now(dt.timezone.utc)
    wall_deadline = (dt.datetime.fromisoformat(args.deadline_utc)
                     if args.deadline_utc else now + dt.timedelta(hours=args.hours))
    if wall_deadline.tzinfo is None:
        ap.error("--deadline-utc must include a timezone")
    remaining_budget = (wall_deadline - now).total_seconds()
    if remaining_budget <= 0:
        ap.error("deadline has already passed")
    deadline = time.monotonic() + remaining_budget
    (run_dir / "manifest.json").write_text(json.dumps({
        "targets": names,
        "deadline_utc": wall_deadline.isoformat(),
        "hours": remaining_budget / 3600,
        "seconds_per_target": args.seconds_per_target,
        "threads": args.threads,
        "starting_draft_sha256": source_hashes,
    }, indent=2) + "\n")
    print("run directory:", run_dir, flush=True)
    print("deadline UTC:", wall_deadline.isoformat(), flush=True)

    env = os.environ.copy()
    env["PATH"] = str(ROOT / "vendor/permuter-venv/bin") + os.pathsep + env["PATH"]
    if not (ROOT / "vendor/permuter-venv/bin/python3").exists():
        print("missing vendor/permuter-venv/bin/python3", flush=True)
        return 2

    pass_no = 0
    matched = set()
    while True:
        pass_no += 1
        searchable = 0
        for name in names:
            if name in matched:
                continue
            remaining = deadline - time.monotonic()
            if remaining < 45:
                record(events, {"event": "deadline", "pass": pass_no})
                return 0
            src = WORK / name / (name + ".c")
            if not src.is_file():
                record(events, {"event": "skip", "name": name,
                                "reason": "source missing", "pass": pass_no})
                continue
            before = digest(src)
            before_text = src.read_bytes()
            seconds = min(args.seconds_per_target, max(30, int(remaining) - 35))
            log = run_dir / (f"pass{pass_no:02d}-{name}.log")
            record(events, {"event": "start", "name": name, "pass": pass_no,
                            "seconds": seconds, "source_sha256": before,
                            "log": str(log)})
            print(f"pass {pass_no} {name}: {seconds}s", flush=True)
            cmd = [sys.executable, "tools/permute.py", name, "--current",
                   "--seconds", str(seconds), "-j", str(args.threads)]
            with log.open("w") as output:
                proc = subprocess.Popen(cmd, cwd=ROOT, env=env, stdout=output,
                                        stderr=subprocess.STDOUT,
                                        start_new_session=True)
                try:
                    rc = proc.wait(timeout=seconds + 90)
                except subprocess.TimeoutExpired:
                    os.killpg(proc.pid, signal.SIGINT)
                    try:
                        rc = proc.wait(timeout=30)
                    except subprocess.TimeoutExpired:
                        os.killpg(proc.pid, signal.SIGKILL)
                        rc = proc.wait()
                    record(events, {"event": "timeout", "name": name,
                                    "pass": pass_no, "returncode": rc})
            after = digest(src) if src.is_file() else None
            record(events, {"event": "finish", "name": name, "pass": pass_no,
                            "returncode": rc, "source_unchanged": before == after,
                            "log": str(log)})
            print(f"  exit {rc}; source unchanged: {before == after}", flush=True)
            if before != after:
                if rc == 0:
                    check = subprocess.run(
                        [sys.executable, "tools/trymatch.py", name],
                        cwd=ROOT, env=env, stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT, text=True)
                    record(events, {"event": "changed_source_check", "name": name,
                                    "pass": pass_no, "returncode": check.returncode,
                                    "output": check.stdout[-1200:]})
                    if check.returncode == 0:
                        matched.add(name)
                        continue
                # Since 2026-08-29 permute.py KEEPS a measured improvement in
                # the draft and says so on its last line. That is the chaining
                # this driver exists for, not corruption; without this branch
                # the first improvement anywhere stopped the whole batch.
                if rc == 1 and "PERMUTE IMPROVED" in log.read_text(errors="replace"):
                    record(events, {"event": "improved", "name": name,
                                    "pass": pass_no, "log": str(log)})
                    print("  improved; the next pass starts from it", flush=True)
                    searchable += 1
                    continue
                # Put the draft back as it was before this run. The changed
                # text is kept beside the log for inspection, never left in
                # the draft where a later trymatch would file it as best.c.
                kept = run_dir / f"pass{pass_no:02d}-{name}.unverified.c"
                if src.is_file():
                    kept.write_bytes(src.read_bytes())
                src.write_bytes(before_text)
                record(events, {"event": "restored", "name": name, "pass": pass_no,
                                "unverified_copy": str(kept)})
                print(f"source changed without a verified match; restored the "
                      f"draft and saved the changed text to {kept}; stopping",
                      flush=True)
                return 3
            if rc != 2:
                searchable += 1
        if searchable == 0:
            record(events, {"event": "stop", "reason": "all setups failed"})
            return 2


if __name__ == "__main__":
    sys.exit(main())
