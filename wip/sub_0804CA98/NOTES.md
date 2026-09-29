
## wave 95

Base: the old draft (kept as `sub_0804CA98.w95-start.c`, 17.1%, size +0, first difference +0xa). Left as the draft.

Pre-registered hypothesis (force-addr word appears when the address is an address VALUE with a variable term, as in c_0804EB78 / c_0804F3C8): REFUTED for this function. The old draft already had the products-plus-`(u8 *)` spelling in its tail, and this wave's rewrite of the whole guarded region in the exemplar spelling (entry pointer `e = a2*sz + a1*grp + (u8 *)g` at the top, `dst` and the two `gUnknown_08552148` entries in the same spelling, `int`-free u16 reads, group stride bound to a `unsigned short` local, tail also in `(u8 *)` spelling, or with the tail by name) produced size -8 / 17.5% (tail `(u8 *)`) or -12 / 14.9% (tail by name). Inlining the entry as a macro instead of a local is byte-identical to the local. In every one of these the `.rodata` word appears for gUnknown_02029B94 only (that is new: the earlier draft had neither); gUnknown_02029A10 stays a plain literal loaded four times.
So the same source shape as c_0804EB78 does not create the word here. Differences between the two: EB78 has a call and other globals' words before the entry, and its entry is used for reads and stores in ONE straight run; this function tests `unk00` and branches to a tail. Unresolved which of those matters.
The `-da` dumps show three surviving `(set P (symbol_ref .LC0))` after cse, which gcse leaves un-unified; the word for B94 is unified. Not chased further.

Proposed summary:
- does: steps a sprite entry's animation counter and, when it wraps, advances the frame and starts the follow-up effect
- status: size-exact draft, ROM holds gUnknown_02029A10 in one held register from a `.rodata` word for the stepping block and a plain literal for the tail
- left: the missing `.rodata` word for gUnknown_02029A10
- tried: entry pointer / inline macro / by-name array / mixed tail spellings (see above); none creates the word for gUnknown_02029A10, though the exemplar spelling does create it for gUnknown_02029B94

## wave 96

Base: the wave-95 draft (`sub_0804CA98.w96-start.c`, 17.07%, size +0). Now `sub_0804CA98.c` = 25.72%, size -12 (404... measured 404/416), first diff +0x1a (was +0xa).

What moved it: the ROM reaches both arrays through their compiler-made cells and loads the VALUE eagerly: `ldr r2,=gUnknown_08136060; ldr r0,[r2]; mov sl,r0` at the top (the array base, held in sl for the guarded region) and `ldr r4,=gUnknown_08136064; ldr r0,[r4]; mov r8,r0` after the unk00 test (the gUnknown_02029B94 base). Declaring the two cells as `extern ... *const gUnknown_08136060 / 08136064`, binding `base = gUnknown_08136060;` at the top of the function and `b94 = gUnknown_08136064;` INSIDE the guarded block, and spelling the guarded region through `base` / `b94`, reproduces those two loads (the draft never created the cells' words at all in its own pool: they are the force-addr words). 17.07% -> 25.72%; the two cell loads and their order now match the ROM. The b94 bind at function top instead of inside the block puts it on the stack (22.6%).

Negatives (each one compile):
- the B80 block's `gUnknown_02029B94[a1][a2] = 1` by plain name (ROM uses a plain literal there): 400 bytes, 15.9%; the `frame = 0` store through plain gUnknown_02029A10 (ROM also a plain literal): 400 bytes, 13.2%; both: 396 bytes, 13.5%. All worse: with them the frame layout changes; the ROM's mix is reached only together with the allocation it has (sl = base held, ip = a1*180, sb = the zero).
- Residual: `base` is spilled to [sp,#20] instead of held in sl (the draft keeps a1*10 in sl), so every later use pays `ldr rX,[sp,#20]`; that accounts for the -12 bytes.

Permuter, 900 s x 2 from this base: 25.72 -> 42.79 but the kept candidate is WRONG C for this repo's rules (adds `volatile unsigned int new_var` — a stack slot — and a `inline_fn(a1*2)` helper); saved as `sub_0804CA98.w96-perm1-WRONG-volatile.c`, not adopted. Useful hint inside it: a `volatile` copy of a2 used once as `b94[a1][new_var]` mimics the ROM's spill of a2*2 to [sp,#0x1c]; the ROM keeps a1*10 / a1*4 / a2*2 as three stack temporaries, so a source with three explicit index temporaries (not volatile) is the next thing to try.

Proposed summary:
- does: steps a sprite entry's animation counter and, when it wraps, advances the frame and starts the follow-up effect
- status: 26% at -12 bytes; the two address cells are now read as the ROM reads them
- left: the array base is spilled to the stack where the ROM holds it in sl; the ROM's three index temporaries (a1*10, a1*4, a2*2) live on the stack
- tried: cell-object binds (the lever), plain-literal spellings for the block and tail stores (worse), permuter (only volatile/inline wrong-C gains)

## wave 97 (W97-X)

Base: draft, size-exact hand edits from levers.py: `done = (frame == frameCount) && a3 != -1; if (done && sub_080153F0(a3))` (5a-309) and `idx = a1 * 10 + cnt * 2` with `idx + 1` for y (5a-150). 25.72% -12 -> 29.33% size-exact (416), frame `sub sp #0x20` equal to ROM. Old draft `sub_0804CA98.w97x-start.c`. Permuter x2 (540 s each, wrongc OK): 29.33 -> 41.59 -> 42.79%, size-exact, first difference +0x1b. Changes it kept: `s.unk06`'s table read hoisted out of the second `if` into the first block (a pure load into a local struct, value used only later, same result; the permuter finds it by scheduling), and a `new_var = gUnknown_08552148` table bind. Read both before promoting.
Rejected: levers.py `1cp-119+5a-139` family (34.9%, "copy of a2 as s16") is genuinely WRONG (a2 = 0xFFFF differs): the 1c' rule changes signedness of a u16 index.
Residual: the array base spill vs sl hold not re-examined; size now matches so the -12 was the `done`/`idx` temporaries.
