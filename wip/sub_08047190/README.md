# sub_08047190

0x08047190, 1292 bytes, THUMB, parked.

Best score so far: 88.8%.

## What it does

Builds the current army's sorted unit list in gUnknown_02028DD8, ended by 0xff. Units are sorted by a per-type rank, then optionally by HP, fuel or ammo, and each transport is followed directly by the units it carries.

## How close it is

Compiles to the right size (1292 bytes); 144 bytes differ (88.9% line up), starting near the top. The remaining difference is register choice and instruction order in the first loop; no statement is missing.

## What is left

The function makes no calls, so one different register choice in the first loop cascades through everything after it. Find the initialisation spelling that gives the original's first-loop registers without growing the function; the current best came from the permuter, so another permuter run from it is a reasonable next step.

## Already tried

- Three separate zero assignments and a bound element pointer in the scans (the older draft): right size but only 18%.
- Chaining `o` with `n` instead of `rank` in the first assignment: the first zero moves resemble the original, but 1296 bytes and 19.0%.
- Pinning `rank` to a fixed register: 1304 bytes, 10.7%.
- `list[n++] = s` as one statement: the original stores before incrementing, so two statements are used.
- A u8 second parameter: the original does not narrow it, so it is int.

## Files

- `sub_08047190.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at exact size 1292/1292, 77.3%. Remaining class is allocation/order after the prior permuter gain. Chained o/n grows to 1296 at 19.0%, fixed-r3 rank to 1304 at 10.7%; older low-scoring and header-regression candidates are rejected.

### Wave 93

WAVE 93 (W93-D): 77.32% -> 88.85%, still size-exact at 1292 bytes, 144 differing bytes, first difference +0x11. The entry's own suggestion (another permuter run from the current best) is right provided the runs are CHAINED: four runs, each started from the previous one's kept improvement, gave 77.32 -> 77.86 -> 87.93 -> 88.85, and the fifth found nothing. The big step was run 2, worth ten points. All three kept changes are semantically neutral and each was read: the type compare in the second scan written with the constant on the left, `if (t == gUnknown_08499594[...].type)`; that same element bound to `e` before the compare (e is assigned before every one of its reads throughout the function, so no stale value is possible); and the loop bound's `+ 1` taken from a u8 local set to 1 immediately before the loop (q3, which is separately re-initialised to 0 before its later use as a counter). drafts.py bases reports no read-before-set and names the draft as the base. WHAT IS LEFT is the same class as before and nothing structural is missing: register choice and instruction order in the first scan, cascading through everything after it because the function makes no calls. The three zero-initialisations at the top land in r5/sl/r9 where the ROM uses r9/sl/r3, and one `ble` moves relative to a `movs`.

</details>
