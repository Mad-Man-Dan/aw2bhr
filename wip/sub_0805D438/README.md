# sub_0805D438

0x0805D438, 436 bytes, THUMB, parked.

Best score so far: 17.7%, +4 bytes (best.c).

## What it does

AI: runs one step of the current army's unit list. It selects the next unit and saves its position, lifts it off the map, calls the AI decision routines, puts it back on the map and advances the list. The end of the list and a special 0x40 entry each have their own path.

## How close it is

Matches byte-for-byte when compiled with -O2 -fno-gcse, that is, with the compiler's global common-subexpression pass off. With the build's normal flags it is 28 bytes too long.

## What is left

Decide whether the original built this function without gcse. Its neighbours sub_0805D5EC and sub_0805D648 need gcse, so a per-file flag would need a file boundary right after this function. The other possibility is a source construct that stops gcse for one function, and none is known.

## Already tried

- Normal flags with the map access as struct Map members: worse (28 bytes too long).
- Normal flags without force-addr, -O1, and the older compiler: none match.
- Binding the unit-list pointer to a local: no change.

## Files

- `sub_0805D438.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

PARKED Wave 70 at 440/436 (+4 section; +12 code and pool 22/24), first difference +0x2. Configured, no-force and O1/no-force profiles bracket but do not match. Binding B0/C0 addresses worsens; Wave 70's u8 overlay folds C0+0x13 into a wrong new pool word. The readable semantic draft is restored; residual is force-address pool selection and its allocation/order cascade. WAVE 82 (W82-C): no-force re-measured myself on the wave-70 draft; jump confirmed, no exit-0 (-4 bytes overshoot persists). Mechanism pinned down from the diff: under no-force the loaded gUnknown_030040D8 pointer VALUE binds to r5 ACROSS both map-store blocks (single ldr r5,[pc] re-read via [r5,#0] at both sites) while the ROM re-materialises it per site under -fforce-addr; simultaneously the pool inventory loses one slot (extra standalone alignment padding appears) so every ldr [pc,#N] displacement shifts even where instruction text is identical, and .word 0x0000417a relocates to a different neighbour. Net: no-force removes precisely the two force-addr slots the ROM KEEPS (gUnknown_030046C0/gUnknown_030046B0 related) rather than reproducing their selective emission. No subset of the seven profiles yields that hybrid; per-site rematerialisation cannot be spelled (volatile-cast and binding probes already measured +12/+28/+24). Override claim NOT made.

### Wave 91

W91-B: PROVISIONAL MATCH under -O2 -fno-gcse (with or without -fforce-addr). The draft in work/sub_0805D438/sub_0805D438.c is byte-identical there ("relocs: match") and is 12.16% / +28 configured. The two source fixes: the struct Map member for both map stores, which gives the ROM's (map+0x417A)+y*2 order, and (u8 *)gUnknown_085766E0 + (type * 12 + 4). The latter replaces gUnknown_085766E0[type].unk04, whose stride became 36 when wave 60 grew struct Unk085766E0 to 0x24. Mechanism, from a -da dump: gcse PRE unifies the .LC address loads of 030046B0/030040D8/030046C0 across the post-call half, and that leaves three .rodata words the ROM does not have. Member form alone, configured: 17.89/+4 -> 12.39/+28, a negative. NOT an override yet: next in address order, sub_0805D5EC and sub_0805D648 fail under -fno-gcse, so the flag needs a unit boundary between D438 and D5EC. Reproduce with python work/sub_0805D438/w91b_tm.py sub_0805D438. See NOTES.md and the codegen chapter.

</details>
