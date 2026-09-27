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
