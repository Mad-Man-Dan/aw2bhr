# sub_0805A9AC

0x0805A9AC, 732 bytes, THUMB, parked.

Best score so far: 67.4%, +4 bytes (best.c).

## What it does

AI: for each unit of type 1 or 2 in the current army that is not yet used and passes a per-mode filter (a1 selects a row of an 8-byte table indexed by bits 3..5 of the unit's byte 9), computes that unit's movement range and finds the reachable, empty tile with the lowest danger value whose terrain the active unit's transport table accepts. The best tile over all units goes to *a2. At the end the unit standing on that tile is counted in gUnknown_03004730.

## How close it is

Compiles to the right size (732 bytes) and is 50.7% identical. The frame is 4 bytes bigger than the original (one value lives on the stack that the original keeps in a register), and the code shifts to make up for it.

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

</details>
