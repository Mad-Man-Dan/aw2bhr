# sub_0802F588

0x0802F588, 280 bytes, THUMB, parked.

Best score so far: 54.6%, +4 bytes (best.c).

## What it does

Sends one packet on the link. Into the 512-entry send ring it writes a 0x4FFF start marker, the length in halfwords (a2 / 2), a checksum (0x4FFF plus the length plus each payload halfword times its 1-based position) and the sum of the complements of those products, then the payload from a1. It returns -1 as soon as the write cursor would catch up with the read cursor; otherwise it stores the new write cursor and returns the halfword count.

## How close it is

Compiles to the right size (280 bytes) but 167 bytes differ (40.4% identical). The code has the original's shape and order; the register choices are rotated through all of its loops from the first few instructions on.

## What is left

Find what makes the compiler pick the original's registers. More unguided permuter runs are not expected to find it; a new idea is needed.

## Already tried

- Two permuter runs to exhaustion: no match. Their best result (45.7%) used a variable before setting it and was rejected.
- Splitting `sum = n + 0x4fff` into two statements and swapping one pointer comparison: kept, but only a small gain (170 to 167 differing bytes).
- Wrapping either checksum update in a do/while that runs once: changes the sum's register, but not to the original's.
- Setting chk before cur: 8 bytes too long (288).
- Reordering the declarations: no change.
- An older permuter result that reaches gUnknown_030040CC through its pointer word scores higher by position but is 4 bytes too long.

## Files

- `sub_0802F588.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

Wave 74 strongest semantic draft: exact 280/280, 40.4%, 167 differing bytes, first difference +0xe.

### What still differs

The ring-writer shape is correct but allocation remains broadly rotated across its three loops.

### Why it is close

Size and loop count are exact; the retained split-sum and reversed second-pointer equality are semantic and independently verified.

### Already ruled out

- Two clean permuter runs were drained.
- The apparent exact 45.7%/152-difference candidate used prod before initialization and was rejected.

### Settled

- Do not prefer best.json's stale 55.4% record over the verified semantic active draft.

### Why it is parked

Wave 74 W74-C. Requires a new allocation mechanism, not another unconstrained permuter adoption.

</details>
