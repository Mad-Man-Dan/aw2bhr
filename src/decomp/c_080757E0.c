#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080757E0.
 * sub_080757E0 @ 0x080757E0
 */

#include "hardware.h"
#include "proc.h"
/* The c_08075C98.c affine block with a CONSTANT denominator: 0x100 is built
 * once and parked in sb across all four Div calls, which is why the angle is
 * re-read from +0x34 each time but the divisor never is.
 * `-SIN_Q12` for pb and plain `SIN_Q12` for pc is the same handedness that file
 * uses; the `lsls #4` on each lut value is the `* 16` it also spells. */
struct Unk80757E0
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u16 unk34;
};

void WorldMapReticle_SpinLoop(struct Unk80757E0 *proc)
{
    proc->unk34++;

    SetObjAffine(0,
                 Div(COS_Q12(proc->unk34) * 16, 0x100),
                 Div(-SIN_Q12(proc->unk34) * 16, 0x100),
                 Div(SIN_Q12(proc->unk34) * 16, 0x100),
                 Div(COS_Q12(proc->unk34) * 16, 0x100));

    PutSpriteExt(5, (proc->unk2c - 0x20) & 0x1FF,
                 ((proc->unk30 - 0x20) & 0xFF) | 0x700,
                 gUnknown_086143D8, 0);
}
asm(".global sub_080757E0\n.thumb_set sub_080757E0, WorldMapReticle_SpinLoop\n");
