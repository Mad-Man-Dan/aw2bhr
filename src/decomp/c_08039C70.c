#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039C70.
 * sub_08039C70 @ 0x08039C70
 */

#include "proc.h"
#include "hardware.h"
/* The animated half of src/decomp/c_08039BDC.c: where that one installs the
 * identity affine slot once, this one eases unk2e towards 0x100 and rebuilds
 * the slot from it every frame, drawing the sprite until unk2e passes 0xf9.
 * The draw half is src/decomp/c_08039DBC.c's verbatim -- same +0x29/+0x2a cell
 * pair, same `<< 4` and camera subtraction, same 0x1ff / 0xff masks -- with the
 * affine index ORed in at bit 9 and the 0x100 that seeded the ease reused as
 * the OBJ attribute bit, which is why one constant lives in sb across the whole
 * function.
 *
 * The ease needs an explicit `u16` TEMP: the ROM's `lsls #0xe; lsrs #0x10`
 * truncates the quotient BEFORE adding the old value, which only a u16 object
 * produces.  Writing `proc->unk2e += (0x100 - proc->unk2e) / 4` (or the same
 * with an explicit `(u16)` cast on the quotient) gives a bare `asrs #2` and the
 * addends the other way round -- shorten_binary_op narrows the ADD, not the
 * division. Measured this wave.
 *
 * `(u8)IsSpriteOnScreen(...)` is a cast at the use, NOT a u8 return type -- see the
 * note in src/decomp/c_08039DBC.c; src/decomp/c_0801306C.c returns `int`. */
struct Unk39C70Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ STRUCT_PAD(0x2b, 0x2c);
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ STRUCT_PAD(0x30, 0x32);
    /* 0x32 */ s16 unk32;
};

void SparkleZoom_Loop(struct Unk39C70Proc *proc)
{
    u16 step;
    int x;
    int y;

    step = (0x100 - proc->unk2e) / 4;
    proc->unk2e += step;

    SetObjAffine(proc->unk32,
                 Div(COS_Q12(0) * 16, proc->unk2e != 0 ? proc->unk2e : 2),
                 Div(-SIN_Q12(0) * 16, proc->unk2e != 0 ? proc->unk2e : 2),
                 Div(SIN_Q12(0) * 16, proc->unk2e != 0 ? proc->unk2e : 2),
                 Div(COS_Q12(0) * 16, proc->unk2e != 0 ? proc->unk2e : 2));

    if (proc->unk2e > 0xf9)
    {
        proc->unk2e = 0;
        FreeObjAffineSlot(proc->unk32);
        Proc_Break(proc);
        return;
    }

    x = (proc->unk29 << 4) - gMap->scrollX + 8;
    y = (proc->unk2a << 4) - gMap->scrollY + 8;

    if ((u8)IsSpriteOnScreen(x, y, 0x40))
        PutOamHi((x & 0x1ff) | (proc->unk32 << 9),
                     (y & 0xff) | 0x100,
                     gUnknown_0849D824, proc->unk2c);
}
asm(".global sub_08039C70\n.thumb_set sub_08039C70, SparkleZoom_Loop\n");
