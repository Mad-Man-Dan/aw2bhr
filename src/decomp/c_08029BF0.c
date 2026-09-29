#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029BF0.
 * sub_08029BF0 @ 0x08029BF0, sub_08029C28 @ 0x08029C28
 */

/* Move the cursor to the object's cell, then derive a duration from unk24:
 * a sixteenth of it, clamped to 0x32 above and floored at 1.
 *
 * The two stores of unk2c are cross-jumped -- the 0x32 arm branches straight
 * into the `str` at the bottom of the else arm -- which is why the `b` lands
 * between the `asrs` and the zero test rather than at the end.
 *
 * `asrs r1, #4` and a `cmp` against 0x3E7 with no narrowing make unk24 a signed
 * word. The struct is local to this file rather than `struct Unk03001470`: the
 * offsets line up, but that type has u16 at +0x24 and +0x26, and this function
 * reads +0x24 with a full-word `ldr`. Recorded rather than reconciled. */
struct Unk29BF0Obj
{
    /* 00 */ u8 filler_00[0x20];
    /* 20 */ s16 unk20;
    /* 22 */ s16 unk22;
    /* 24 */ int unk24;
    /* 28 */ u8 filler_28[0x04];
    /* 2c */ int unk2c;
};
/* A two-field forwarder. Both reads are plain `ldrh`, where sub_08029BF0 reads
 * the SAME two offsets with `ldrsh` -- sub_0802723C takes `int` and would have
 * sign-extended an s16 field, so as far as this unit can tell the fields are
 * unsigned here. The two readings are recorded, not reconciled; a third caller
 * would settle it. */
struct Unk29C28Obj
{
    /* 00 */ u8 filler_00[0x20];
    /* 20 */ u16 unk20;
    /* 22 */ u16 unk22;
};

void sub_08029BF0(struct Unk29BF0Obj *p)
{
    ScrollCameraToKeepCellInView(p->unk20, p->unk22);

    if (p->unk24 > 999)
    {
        p->unk2c = 0x32;
    }
    else
    {
        p->unk2c = p->unk24 >> 4;

        if (p->unk2c == 0)
            p->unk2c = 1;
    }

    sub_0803B4DC(0x6B);
}

void sub_08029C28(struct Unk29C28Obj *p)
{
    sub_0802723C(p->unk20, p->unk22);
}
