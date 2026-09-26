# AW2:BHR decompilation agent guide

This repository is a byte-matching decompilation of Advance Wars 2: Black
Hole Rising. A function is complete only when agbcc reproduces the original
bytes and the integrated ROM still has SHA1
`14dd0b22c894865867aff89e8116b2dffae25605`.

The upstream Claude wave documents were not published with the repository.
Use these preliminary, tracked replacements so a fresh checkout is sufficient:

- `docs/decomp-wave/SKILL.md`
- `docs/decomp-wave/brief-template.md`
- `docs/decomp-wave/agent-prompts.md`

Read them before orchestrating or participating in a decompilation wave.
They summarize this guide; this guide remains authoritative unless the user
instructs otherwise. Record measured workflow corrections durably. Do not
remove or replace any existing files under `../.claude/`.

## Repository boundaries

- Work in this directory on `main`.
- Never push. `origin` is upstream, not the user's destination.
- Never edit `asm/`; it remains the immutable reference.
- Preserve unrelated user changes and inspect `git status` before integration.
- Builds run in WSL. Per-function matching tools run from the project-local MCP
  server, Windows Python, or directly with `python3` in Linux/WSL.

## Current operating model

- Ask the user for the concurrent matching-agent cap before each wave.
- The coordinator owns preflight, batching, the target ledger, independent
  verification, promotion, generated files, both builds, reindexing, progress,
  parking reconciliation, and the commit.
- Matching agents own explicit, non-overlapping target lists and their
  `work/<fn>/` drafts. They do not promote or commit.
- Give a matching agent 5-6 functions from one region/vocabulary. Anything
  above roughly 1,500 bytes gets a dedicated agent.
- Prefer a fresh agent when moving to an unrelated region. Retask only when the
  same structs, globals, callees, or compiler configuration genuinely carry.
- At more than three matching agents, one slot may investigate a concrete
  question. At three or fewer, attach the experiment to a matching batch with
  an explicit acceptance test.

## Selecting work after Wave 60

The exact-family, shape-similarity, and broad duplicate wells are nearly dry.
Do not run a giant address sweep or rank work by percentage alone.

Use this priority order:

1. Re-measure drafts excluded by the residual screen because their candidate
   binaries are missing or stale.
2. Target unspent small residuals, especially loop and straight-line drafts
   whose generated code length is within 2-4 bytes of the ROM.
3. Take coherent toolchain-sensitive blocks where compiler overrides are
   evidence-backed.
4. Work size-exact allocation/order residuals selectively; a high score often
   means an unreachable instruction-order decision rather than an easy match.
5. Give large incomplete transcriptions and functions over 1,500 bytes their
   own agents.
6. Sweep trivial return stubs as a bulk, signature-aware task rather than one
   agent per stub.

`python tools/preflight.py` now prints the current residual queue automatically.
Run `python tools/overlap_screen.py` for the full locality/shape view; its
post-wave-61 defaults are `--min-size 24`, loop-inclusive,
`--block-min-matched 1`, and `--top-blocks 12`. Use `--no-loops` only to
reproduce a historical straight-line screen. Treat `best.json` percentages as
leads only and re-test by exit status; preflight labels nonzero size deltas so a
100% prefix with an extra helper body is not mistaken for a match.

Before parking a source-level miss, inspect `data/compiler-overrides.json` and
the surrounding block. Wave 60 found whole groups whose source was already
correct under a different compiler or optimization level.

## Matching-agent rules

- Read the current wave brief in full, then read only the relevant chapters of
  `docs/agbcc-codegen.md`.
- Check every target against `data/parked.json` and
  `data/asm-resident.json` before working it.
- Read the named promoted exemplar before the target assembly when one exists.
- Use the project MCP tools for independent lookups, `compile_probe`, and
  `try_match`; batch independent lookups into parallel calls.
- Both compile tools accept temporary compiler profiles: `default`, `no-force`,
  `o1`, `o1-no-force`, `old-agbcc`, and `old-agbcc-no-force`. These
  experiments write profile-specific artifacts and do not update canonical
  residuals or `best.c`. A profile match is provisional: report the mechanism
  to the coordinator, record an evidence-backed override, regenerate
  `build/compiler-overrides.mk`, and re-run the `configured` verdict.
- To test ONE flag, use `trymatch.py <fn> --cflags-add=-fno-gcse` (or
  `--cflags-remove=`; MCP `cflags_add` / `cflags_remove`). Before proposing
  an override, run `python tools/flag_probe.py <fn> --cflags-add=...`: it
  compiles the neighbours with the same change and names the nearest
  promoted functions it breaks, which bounds the file the flag could apply to.
- A RAM address the code reads with no linker symbol cannot be named in C.
  `python tools/add_ram_symbol.py 0x0200C618` adds the symbol to
  `aw2bhr.lds`; then declare it in `include/unknown-globals.h`.
- Start from the existing draft. Run it once, read the current diff, and
  classify the residual before editing. Do not discard prior measurements.
- Budget about ten tool-call turns per function. At the budget, record the
  exact remaining diff and ruled-out axes, then move on.
- Leave the final source at `work/<fn>/<fn>.c`. A report is not proof of a
  match; only an independently observed zero exit status is.
- Record durable code-generation discoveries in `docs/agbcc-codegen.md` and
  evidence-backed type/prototype corrections in the appropriate header.
- Write every comment and parked `summary` to `docs/writing-notes.md`: say what
  the code does, in plain words, for a programmer new to the project. Match
  history (wave numbers, agent names, scores, register diffs, failed
  spellings) goes in the commit message, never in `src/` or `include/`. After
  editing comments in a promoted file, run `python tools/sync_work.py <fn>` so
  the draft in work/ carries the same text, and prove the edit touched
  comments only with `python tools/comment_check.py`.
- Preserve the starting draft before any permuter run and re-run `trymatch.py`
  on any reported result. `best.json` records whether the permuter wrote
  `best.c` (`"origin"`); `permute.py` will not start from such a file unless
  given `--from-permuter-best`. `python tools/drafts.py audit` re-checks
  every parked `best.json` against its `best.c`. The MCP wrapper's missing-`exit_code` timeout crash
  was fixed after Wave 61; a server-side timeout is now an ordinary exit 124
  failure. The Codex MCP client timeout in `../.codex/config.toml` must also be
  at least 600 seconds for a normal 300-second run.

## Shared-file coordination

The main collision surfaces are:

- `include/unknown-functions.h`
- `include/unknown-globals.h`
- `docs/agbcc-codegen.md`
- `data/parked.json`
- `data/compiler-overrides.json`

Prompts must identify expected cross-batch symbol collisions. One retry is
reasonable for an apparent torn header read; if the failure persists, run
`tools/proto_check.py` and treat it as real. Do not reshape a shared struct
without checking every promoted definition and caller affected by it.

Only the coordinator edits or regenerates integration state such as
`data/promoted.json`, `data/functions.json`, `data/families.json`, linker
scripts, `src/decomp/` promotion outputs, and `progress/index.html`.
`data/parked.json` must retain CRLF line endings and one-space JSON indentation.
Do not round-trip `data/asm-resident.json`; it contains evidence that must not
be silently normalized or discarded.

## Verification and integration gates

1. The coordinator writes one exact, deduplicated target list.
2. Run `python tools/verify_batch.py <list> --log <log>` after all matching
   agents are idle. Trust only reconciled `verified N of N` output and each
   function's exit status. `FAIL(2)` means no usable draft; `FAIL(1)` means a
   draft exists but misses.
3. Re-verify any already-promoted unit affected by shared header or type edits.
4. Run `tools/proto_check.py`, then promote only independently verified
   matches with `tools/promote.py`.
5. Regenerate in order: `split_asm.py`, `split_rodata.py`, `gen_lds.py`.
6. In WSL, delete `aw2bhr.gba` and `aw2bhr.elf` between routes. Run both
   `make SPLIT=1 compare` and `make compare` with independent exit codes and
   pipe failure propagation. Both must report the canonical ROM SHA1.
   In a Codex sandbox that denies direct WSL access, use the project MCP
   `rom_compare` tool instead; it runs those same two fixed routes, deletes the
   shared outputs before each, and reports the exit/SHA1 result separately.
7. Only after both builds pass, run `index_functions.py`, `families.py`, and
   `progress_map.py`; reconcile parked entries, run `export_wip.py` to refresh
   the tracked `wip/` drafts, and inspect the final diff.

The checked-in README progress can lag the machine tracker. Use
`data/functions.json`, the generated progress page, and the latest wave commit
when briefing agents.

The progress page's headline is the same decomp-index metric used above. Its
larger memory map separately classifies pre-existing upstream C, active asm,
parked drafts, asm-resident code, and FE-identified active assembly; do not add
upstream C to the decomp numerator.
