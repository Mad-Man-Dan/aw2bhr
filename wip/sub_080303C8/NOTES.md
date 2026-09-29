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
