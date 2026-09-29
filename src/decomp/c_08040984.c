#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040984.
 * SiloFire_ScrollToSilo @ 0x08040984
 */

struct Unk40984Proc
{
    /* 00 */ u8 filler_00[0x64];
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
};

/* MATCHED. Byte-for-byte the same function as CannonFire_ScrollToTarget -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void SiloFire_ScrollToSilo(struct Unk40984Proc *p)
{
    ScrollCameraToKeepCellInView(p->unk64, p->unk66);
}
asm(".global sub_08040984\n.thumb_set sub_08040984, SiloFire_ScrollToSilo\n");
