# sub_0802F6A0

0x0802F6A0, 604 bytes, THUMB, parked.

Best score so far: 26.3%.

## What it does

Reads one packet for player `slot` out of that player's column of the link receive ring. It walks that player's read cursor forward to the next 0x4FFF start marker, checks that enough halfwords have arrived, reads the length (0x80 halfwords at most), the expected checksum and its complement, then copies the payload into dst while recomputing both. It returns the length in bytes, or -2 when not enough data has arrived, -3 when the checksums disagree and -4 when there is no usable packet.

## How close it is

Compiles to the right size (604 bytes); 445 bytes differ. The control flow, both loops and all the exits are right. CAUTION: the 45.9% recorded by earlier waves was a score for source that did the wrong thing -- it indexed the cursor table and the ring column by the wrapped cursor where the original uses the player slot. The draft is now faithful and 26.3% is its real score.

## What is left

One allocation difference. The original keeps dst on the stack, reloading it once before the copy loop, and keeps the address of the compiler's own map of the cursor table in a register across the first three blocks; our build does the opposite. Those are one fact: the original carries one more long-lived value, and that is what pushes dst out to the stack.

## Already tried

- Binding the wrapped cursor to a local inside the branch that needs it: 4 bytes too long.
- Writing the wrap-around as a conditional expression with the subtraction outside it: 8 bytes short.
- Copying dst into a local before the loop (earlier wave): dst does reach the stack, but the stack frame grows.
- Widening the expected checksum from u16 to int (earlier wave): no change.

## Files

- `sub_0802F6A0.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

Wave 74 strongest semantic draft: exact 604/604, 45.9%, 327 differing bytes, first difference +0xc.

### What still differs

The three-loop ring reader remains blocked on the dst/expSum register-versus-stack inversion.

### Why it is close

The retained nested cursor binding is the strongest independently verified semantic source and is size-exact.

### Already ruled out

- Two clean permuter runs were drained.
- The apparent 49.7/49.8% candidates assign locals after an unconditional return while earlier goto paths read them; both were rejected.

### Settled

- Do not quote or install the semantically invalid best.c candidates.

### Why it is parked

Wave 74 W74-C. Resume at the dst/expSum live-set inversion.

### Wave 92

- **agent:** W92-A
- **correction:** THE INHERITED 45.86% WAS A SCORE FOR SEMANTICALLY WRONG SOURCE and must not be quoted again. The wave-74 draft indexed both gUnknown_03003128 and the gUnknown_02025C18 column by the wrapped cursor (and wrote gUnknown_02025C18[t][t]); the ROM indexes both by slot. Verified at two independent points in the listing: at 0x0802F78E the cursor table is indexed by (s8)slot*2, and at 0x0802F7B2 the same slot*2 is added to cursor*8 and the ring base, so the ring is gUnknown_02025C18[cursor][slot] -- 1024 rows of four halfwords, one per player.
- **measured:** Faithful draft: 604/604 size-exact, 26.32%, first difference +0xc. THAT IS THE REAL BASELINE. best.c's 49.83% is the other artefact wave 74 already rejected.
- **left:** One allocation fact. The ROM spills dst at entry (str r1,[sp,#4]) and reloads it once before the copy loop, and keeps the address word's address in sl across the first three blocks; our build keeps dst in r8 and rematerialises the address word. Those are one fact: the ROM carries one more long-lived value, which is what pushes dst to the stack. Frame size (12 bytes) and slot's slot (sp+0) already agree; only sp+4 / sp+8 are swapped.
- **tried:** - Naming gUnknown_03003128 / gUnknown_03003F48 / gUnknown_02025C18 directly (already in the draft) reproduces the ROM's pool structure exactly, including agbcc's own .rodata address triple with addends 0/4/8 where asm/ prints gUnknown_08090C8C/90/94.
- Binding the wrapped cursor to an int local inside the taken arm: 608 bytes (+4), 19.41%.
- Rewriting the wrap as a conditional expression with the subtraction outside: 596 bytes (-8), 18.71%.
- **next:** Permuter from THIS draft -- size-exact, instruction order correct, pure allocation residual. Not from best.c and not from the wave-74 draft.
- **permuter:** One 900 s run from the faithful draft produced 854 output directories, because a weak base makes almost any mutation an improvement by permuter score. tools/permute.py's harvest has no cap and verifies every output spliced and raw, so the verification phase would have run for hours with the draft held as 204 KB of header-expanded source throughout. The run was stopped during harvest, the process confirmed gone, and the draft restored from work/sub_0802F6A0/w92-faithful.c and re-verified at 604/604, 26.32%. The 854 outputs are kept; a future wave can check the LOWEST-scoring ones without re-running the search. FOR THE TOOLING: cap the harvest, or verify in score order under a time budget.

</details>
