# sub_0805A9AC

0x0805A9AC, 732 bytes, THUMB, parked.

Best score so far: 67.4%, +4 bytes (best.c).

## What it does

AI: for each unit of type 1 or 2 in the current army that is not yet used and passes a per-mode filter (a1 selects a row of an 8-byte table indexed by bits 3..5 of the unit's byte 9), computes that unit's movement range and finds the reachable, empty tile with the lowest danger value whose terrain the active unit's transport table accepts. The best tile over all units goes to *a2. At the end the unit standing on that tile is counted in gUnknown_03004730.

## How close it is

Compiles to the right size (732 bytes) with 65.2% of bytes identical. What is left: the draft's frame is 40 bytes where the original's is 36.

## What is left

Register allocation. In the original the map pointer's address inside the inner loop gets no register and is reloaded from the literal pool at each use, while the column counter keeps sl. In the draft the two nearly tie and the address wins. Chain the permuter from the current draft.

## Already tried

- Writing the map reads as struct Map members or through gMap: no change.
- Volatile reads of the map pointer at single sites: the size stays wrong.
- Compiling with the gcse pass off: the original's three literal-pool words disappear, so this function was built with gcse.
- Permuter, 15 minutes: found the current size-exact draft (a map pointer local set at the top of the inner loop). A +4 candidate at 68% was semantically wrong (a do/while(0) that changes what continue does) and was discarded.

## Files

- `sub_0805A9AC.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

PARKED Wave 78 at 760/732 (+28 section, +22 code), 37.6%. The surplus gUnknown_08499590 force-address word and spill cascade remain after the documented address variants. Preserve the readable baseline.

### Wave 91

W91-B. Member form: NEGATIVE. The draft already uses a struct Map5A9AC cast, and respelling it as struct Map or gMap (->move included) is byte-identical at 37.57/+28. Mechanism of the +28, from a -da dump (work/sub_0805A9AC/rtl-w91): the surplus .LC1 is GCSE PRE (insn 987, inserted after the n-loop init, with every in-loop map access now a copy of it), so combine cannot fold it into plain ldr =gUnknown_08499590 words. The in-loop map-address pseudo (16 refs / 160 insns) then beats i (25 / 256) for sl by about 2% of priority, and i spills. In the ROM the address pseudo loses and is rematerialised per use from REG_EQUIV, which is why the ROM's four plain loads land in four different registers. The ROM does use gcse: its prologue .rodata words exist. A 900 s permuter run from --current produced the FIRST SIZE-EXACT candidates: output-10764-1 at 50.68%, which binds new_var = (struct Map5A9AC *)gUnknown_08499590 at the top of the j body (transcribed as work/sub_0805A9AC/w91-perm10764-clean.c), and output-9844-1 at 40.98%, which reads the width into a u16 before the loop. Chain the permuter from the clean transcription. UPDATE after the run finished: the draft is now the clean transcription of output-10764-1. trymatch verifies it at 732/732, 50.68%, with the first difference at the frame size (sub sp,#40 against #36). The run's own "IMPROVED" pick (output-8096-1, 67.76%, +4) wraps the loop body in do{}while(0), which changes what continue does, so it was discarded as semantically wrong. The previous +28 draft is kept as work/sub_0805A9AC/w91-start.c.

### Wave 96

Base: the 50.68% draft (`sub_0805A9AC.w96-start.c`, first +0xa, frame `sub sp,#40` vs ROM #36 because `i` spills).
Un-binding, measured over all 32 subsets of the five `new_var->` sites (one unit, size + try_match):
- Un-binding ONLY the last one (the post-loop `bv = ...` read after the n-loop, where `new_var` was a stale bind
  from the j loop) gives the ROM's frame (`sub sp,#36`), size 728 (-4), first difference moves +0xa -> +0x3e
  (score drops 50.7 -> 42.3 because the size shift moves later bytes). Kept as the draft and as `w96-m16.c`.
  Mechanism: that use kept the map-address pseudo alive across the whole n-loop, which is what beat `i` for sl.
- Un-binding the first two sites (unk2D5A guard, danger index) changes nothing (same bytes).
- Un-binding site 3 or 4 (the `unk1432` sites) is worse (+8 / first +0xa again).
- Un-binding all five and deleting the bind: 760 (+28), frame 44.
- Removing the volatile read (plain cast at that site): 724 (-8), same frame.
Residual after the kept change: the running best (`best.raw`) sits in r9/r4 with a `mov r4,r9; ands; orrs; mov r9,r4`
round-trip where the ROM keeps it in r6; the map address is held in r8 (`mov r8,r1; mov r6,r8`) where the ROM
re-derives `ldr r7,=gUnknown_08499590; ldr r2,[r7]` per site (rule 1: re-derive form); and the 4 missing bytes.
NOT yet tried: re-assigning a pointer-to-cell local at the top of each of the four arms (rule 1 form).
Proposed summary: left: map address held in a hi register (ROM re-derives per use); `best` in r9 not r6; -4 bytes.
tried += new_var unbound at the post-loop use (frame now matches).
Rule-1 re-derive probe (one unit, on the kept form): rebinding `gp = &gUnknown_08499590` per use inside the j body
(expression form `(*(gp = (struct Map5A9AC **)&g))`): all sites 736 and frame 40 (worse); only the first site 728, frame 36 (= kept form);
volatile site only 728 frame 36; unk1432 sites only 732... no variant reaches 732 with frame 36. The re-derive form does not transfer here
because the ROM's per-site `ldr r7,=gUnknown_08499590; ldr r3,[r7]` uses the plain pool word, not a rodata cell.

### Wave 97

wave 97 (W97-X)
Base: levers.py `3a-206+5a-339` (loop step through a copy `nj = j + 1; ... j = nj`, plus the `unk1432` tile read bound to a local before the sub_08026FD0 call), hand-written into the draft (old draft `sub_0805A9AC.w97x-start.c`, the nj-only form `sub_0805A9AC.w97x-nj.c` is the draft). 42.35% -4 -> 65.16% size-exact (732), first difference still +0xa, frame `sub sp,#40` (ROM #36: nj takes a slot).
The `tile` local alone changes nothing (65.03% without it). wrongc says WRONG on this family (extra sub_08042D1C call on seed 117, args (0x41DA63EB, 0x66)): a1 = 0x41DA63EB indexes `t[a1]` far out of range, so the emulated stack contents differ between two frames and the read differs; I believe this is a false positive but could not prove it. The old w96-start draft gets the same verdict against the current base, and the base against itself is OK. Treat 65% as unconfirmed until someone reads seed 117.
Without nj (va: j++) the frame matches (`#36`) but size is 728 and first diff +0x3e. The 4 bytes are the ROM's `adds r4,#1` step vs the draft's stack-resident nj; the ROM has j in r4 with no copy, so the +4 is elsewhere (the map address held in r8 where the ROM reloads `ldr rN,=gUnknown_08499590` at each site, and the j-loop bound reload is a plain pool load in the ROM).

wave 97 (orchestrator check)
The W97-X draft (65.16%) is equivalent C by reading: nj copy-back step (int) and `u8 tile` for a u8 array element passed to a u8 parameter. wrongc WRONG on seed 117 comes from an out-of-range random index reading stack memory whose layout differs between the two frames (0x28 vs 0x24), not from the source change. Kept.

</details>
