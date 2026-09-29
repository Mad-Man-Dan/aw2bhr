# Wave 93 (W93-C)

Unchanged at 420/428 (-8), 17.99%, first difference +0xe.

## best.c rejected, renamed best.c.wrongc

19.63% and size-exact, but its first difference is +0xa against the draft's
+0xe: it diverges EARLIER. The percentage is higher only because it is 8
bytes longer while the draft is short. On a short draft the percentage is not
comparable across sizes. Compare the first-difference offset.

## Two ROM constructs measured, both byte-neutral

The ROM's key word ends:

    movs r4, #0x80 ; lsls r4, r4, #8   <- 0x8000 built in r4
    adds r2, r4, #0                    <- and COPIED, i.e. held in a register
    orrs r0, r2
    ...
    ldrh r0, [r3, #6]                  <- dead read of unk06
    strh r1, [r3, #6]

Both look like missing source. Neither is:

    int hi = 0x8000; used in the grouping              18.46%  still -8
    a discarded read `gUnknown_0849B01C->unk06;`       16.82%  still -8
    both together                                      17.29%  still -8

THE DRAFT ALREADY EMITS THE ROM'S DEAD READ. Every member of struct
Unk0849B01C except unk08 is volatile, so the read falls out of the existing
assignment; authoring a second one adds nothing. The `hi` local reproduces
the ROM's grouping, as the park says, and costs nothing either.

So the missing 8 bytes are NOT the visible `adds r2,r4,#0` and dead `ldrh`.
Those are already accounted for. The residual is the two `unk210 |= 0xFFFF`
sites, as the park says, and the spellings it lists are still the only ones
measured.

## wave 95

Base: existing draft (420, -8, 18.0%), kept as `sub_080303C8.w95-start.c`; draft unchanged.
- Lever 1: the ROM copies (`adds r3,r2,#0` / `adds r2,r4,#0` before the wait loop) hold the addresses of gUnknown_0849B018 and gUnknown_02023894, each loaded once through the compiler's .rodata address words. Binding those addresses to locals (`pi = &gUnknown_02023894; pa = &gUnknown_0849B018;`, loop reads `*pi`, `(*pa)->unk04`) is byte-identical to the draft: the copy propagates away. Lever did not transfer; mechanism: the ROM's copies are loop-rotation copies the compiler makes, and a source local is just propagated. The first difference (+0xe) is the ROM holding the address in r4 (ours r2), which a source local does not change.
- unk210 read-modify-write: `w = unk210; unk210 = w | 0xFFFF;` byte-identical (combine forwards the read into the use, the dead first read stays: still two reads). `w = unk210 | 0xFFFF; unk210 = w;` loses BOTH reads (412 bytes). With `w` also used later (`acc += w`) identical to the draft. So a single volatile read before the write is not reachable by a local temp; the lever chapters (narrow-global volatile read) were not enough here.
- Not tried: permuter (draft is 8 bytes short in size).

Permuter (added at end of wave 95): two chained 600 s runs from the draft, 18.0 -> 40.9 -> 48.6, SIZE-EXACT (428), first difference +0xa. Draft = current `sub_080303C8.c` (= `.w95-perm3-start.c`). Changes, checked by reading:
1. `new_var = gpKeySt;` read at the top and `return new_var->previous;` at the end. Differs from the start only if gpKeySt is reassigned during the function (callees sub_080301E8 / sub_0802F460); NOT verified, so treat as provisional. This is what supplied the missing 8 bytes (a live saved-register copy).
2. `new_var2 = 2;` holds the constant in the wait loop's `unk04 != 2` test.
3. In the key word the OR is written with the `0x8000 | unk00 << 10` group first, then `~REG_KEYINPUT & 0x3FF`, then `unk02 << 13` (the same value; the ROM groups 0x8000 with the shifted field, which this ordering reproduces without the local for 0x8000).
Lever 1 (copy of a value into a saved register): transferred through the permuter's `new_var = gpKeySt` form; my hand-written address locals were folded away. Remaining: unk210 read-modify-write still reads twice, r4 vs r2 for the &gUnknown_0849B018 address.

## wave 96

Base: wave-95 permuter draft (428, 48.6%), kept as `sub_080303C8.w96-start.c`; draft unchanged at the end.

gpKeySt check (the item wave 95 left open): nothing in the tree writes gpKeySt except the one-time init in src/decomp/c_08013434.c,
and asm/ only loads it, so the top-of-function read is semantically harmless. BUT it is not what the ROM does: the ROM reads gpKeySt
once, at the very end (`ldr r0,=gpKeySt; ldr r0,[r0]; ldrh r0,[r0,#6]` in the shared tail). Reading it at the end is the faithful
spelling and scores 15.4% at 420 bytes (-8): the top-of-function read only pays for the missing 8 bytes by keeping a saved register
alive, it does not describe the ROM. So the 428 draft is size-exact by an unfaithful route; the 8 bytes still need their real source.
Where the 8 bytes really are (diff of the faithful form): the wait-loop's two rotation copies (`adds r3,r2,#0` / `adds r2,r4,#0`,
4 bytes; the ROM keeps &gUnknown_0849B018 in r4 from the first load), and the ROM's two `unk210 |= 0xFFFF` sites carry ONE read
where ours carry two (the 4 bytes the draft has extra are offset by 12 it lacks elsewhere).
Negatives, each with mechanism (all -12 or unchanged):
- `*(volatile u16 *)&...->unk210 |= 0xFFFF`, or through a `volatile u16 *` local: the pointer-rooted store loses the dead load (that is
  the wanted effect, see the wave-49 chapter) but the constant OR folds to a plain `strh 0xFFFF` (-12). The member-rooted store keeps
  the OR and the extra dead load. The ROM has the OR AND no dead load: it needs a pointer-rooted store whose value is not foldable.
- `unk210 = 0xFFFF | unk210` is byte-identical to `|=`.
Note the ROM's second site ORs with r5, the same register that holds the `== 0xFFFF` compare constant (built as `0xFFFF0000 >> 16`),
so the source constant there is a variable/cse value, not a literal; the first site loads a literal from the pool.

Permuter (wave 96, one 600 s run from the 48.6% draft): 48.60 -> 49.77%, size-exact 428. The kept change is right C: the bad-packet
site's `unk210 |= 0xFFFF` uses a local `allOnes = 0xFFFF` (renamed from new_var3), which matches the ROM ORing with a live register
(the same register that holds the `== 0xFFFF` compare constant) at that site while the first site keeps the pool literal. Adopted.
Remaining: wait-loop copies / r4 vs r2 for &gUnknown_0849B018, the second dead `ldrh` at the first unk210 site, gpKeySt read at the top
where the ROM reads it at the end (see above).
Proposed summary: does = link handshake and key-word exchange; status = 49.8%, size-exact by an unfaithful gpKeySt read;
left = 8 bytes' real source (wait-loop copies, single-read unk210 RMW); tried = pointer-rooted volatile stores (fold the OR away),
faithful late gpKeySt read (-8), address locals (folded), permuter x3 across waves 95-96.

## wave 97 (W97-Y)
Base: the FAITHFUL end-read form (no early `new_var = gpKeySt;`, `return gpKeySt->previous;` at the shared tail), frameless like the ROM. Draft kept as `sub_080303C8.w97y-start.c`. The old 428-byte draft carries `sub sp, #4` (a spill slot for the early gpKeySt read) which the ROM does not have: its size-exactness was that slot, confirmed by `spellings.py` (draft: frame #4; ROM and faithful form: no frame).
Lever that made the faithful form size-exact: the wait loop as `if (unk06 != 0 && gUnknown_02023894 == 0) { do { if (gUnknown_02023894 != 0) break; } while (unk04 != 2); }` (loop-inverted like the ROM's `adds r3,r2,#0 / adds r2,r4,#0` entry) -> 428 bytes, frameless, 31.78% (was 15.4% at 420). The `while (X == 0 && unk04 != 2)` spelling and the `while(1) { break; break; }` form are 420 / 436.
Permuter (3 links, wrongc OK each, diffs read): reads `gUnknown_0849B01C->unk00` once per iteration into `new_var2` before the unk208 tests (a volatile RAM read moved earlier on the early-out path, no other effect) and hoists `unk02 << 13` into `i` before the v expression (i is re-initialised by the for). Now 54.21%, 428 exact, frameless, first diff +0x1c (draft +0xA). Value-neutral but the two hoists are spellings, not the ROM's source shape.
Residual: v expression order (ROM reads the keypad, then unk00, then unk02; here unk02 and unk00 come first), the loop-top address regs, unk210 RMW sites unchanged from wave 96.
Proposed status: 54.2%, size-exact WITHOUT the unfaithful early gpKeySt read; left = v-expression operand order and the wait-loop/loop-top register roles.
