#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039DBC.
 * sub_08039DBC @ 0x08039DBC
 */

#include "proc.h"
/* A scripted 18-frame approach: the sprite's x steps 0x10 on seven scattered
 * frames of a jump-table switch, and frame 20 takes the last step, resets the
 * counter and unblocks the script instead of drawing.
 *
 * The switch is dense (unk2e 3..20) so agbcc emits a real jump table; the
 * unlisted frames share the table's default slot with the listed ones' `break`
 * target, which is why the seven step cases and the default arrive at the same
 * label. Case 20 `return`s -- it must skip BOTH the draw and the unk2e
 * increment, and a `break` would fall into them.
 *
 * THE `(u8)` ON IsSpriteOnScreen IS A CAST AT THE USE, NOT THE RETURN TYPE.
 * src/decomp/c_0801306C.c returns `int`; the bare `lsls #0x18` here is a
 * low-byte truth test written in the source. Reading it as a `u8` return
 * instead is byte-identical at this one call site and disagrees with the
 * promoted definition -- tools/proto_check.py catches that, try_match cannot.
 *
 * The camera origin is read TWICE, once for the on-screen test and once for
 * the draw, because they are separate expressions; only the address of
 * gUnknown_08499590 is held (in r7) across both. +0x29 and +0x2a each need a
 * runtime `adds` because ldrb's displacement stops at 31. */
struct Unk39DBCProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ STRUCT_PAD(0x2b, 0x2c);
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
};

void SparkleEffect_StepFrames(struct Unk39DBCProc *proc)
{
    switch (proc->unk2e)
    {
    case 3:
    case 6:
    case 9:
    case 12:
    case 14:
    case 16:
    case 18:
        proc->unk2c += 0x10;
        break;

    case 20:
        proc->unk2c += 0x10;
        proc->unk2e = 0;
        Proc_Break(proc);
        return;
    }

    if ((u8)IsSpriteOnScreen((proc->unk29 << 4) - gMap->scrollX,
                         (proc->unk2a << 4) - gMap->scrollY,
                         0x40))
    {
        PutOamHi(((proc->unk29 << 4) - gMap->scrollX + 8) & 0x1ff,
                     ((proc->unk2a << 4) - gMap->scrollY + 8) & 0xff,
                     gUnknown_0849D81C,
                     proc->unk2c);
    }

    proc->unk2e++;
}
asm(".global sub_08039DBC\n.thumb_set sub_08039DBC, SparkleEffect_StepFrames\n");
