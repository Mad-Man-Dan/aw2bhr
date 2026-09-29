#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F1D8.
 * CannonFire_ScrollToTarget @ 0x0803F1D8
 */

struct Unk3F1D8Proc
{
    /* 00 */ u8 filler_00[0x64];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};

/* MATCHED, and the first of a byte-identical pair with SiloFire_ScrollToSilo. Forwards
 * a proc's +0x64/+0x66 coordinate pair to ScrollCameraToKeepCellInView. Both members are
 * `ldrsh`, so both are s16, and ScrollCameraToKeepCellInView's own `lsls #0x10; asrs #0x10`
 * pair on each argument is the parameter narrowing, not a cast here. */
void CannonFire_ScrollToTarget(struct Unk3F1D8Proc *p)
{
    ScrollCameraToKeepCellInView(p->unk64, p->unk66);
}
asm(".global sub_0803F1D8\n.thumb_set sub_0803F1D8, CannonFire_ScrollToTarget\n");
