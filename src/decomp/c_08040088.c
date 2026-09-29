#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040088.
 * sub_08040088 @ 0x08040088
 */

/* Family F025-adjacent: `f(proc->unk2c, proc->unk30)` with both coordinates
 * loaded `ldrsh`. The two fields are s16 OBJECTS and not (s16) casts of int
 * members: `movs rI,#0x2c / ldrsh rD,[rB,rI]` is the s16-object tell -- an int
 * member converted to ScrollCameraToKeepCellInView's s16 parameters would emit `ldr` and let
 * the callee narrow, and an (s16) cast on an int member would emit
 * `ldr; lsls #16; asrs #16`. Same shape as SiloFire_ScrollToSilo next door, at 0x2c/0x30
 * instead of 0x64/0x66. */
struct Unk40088Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ s16 unk2c;
    /* 2e */ u8 filler_2e[0x02];
    /* 30 */ s16 unk30;
};

void ExplosionEffect_ScrollToCell(struct Unk40088Proc *proc)
{
    ScrollCameraToKeepCellInView(proc->unk2c, proc->unk30);
}
asm(".global sub_08040088\n.thumb_set sub_08040088, ExplosionEffect_ScrollToCell\n");
