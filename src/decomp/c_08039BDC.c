#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039BDC.
 * sub_08039BDC @ 0x08039BDC
 */

#include "proc.h"
#include "hardware.h"
/* Allocates an OBJ affine slot and loads it with the identity rotation at
 * scale 1. The angle is the literal 0, so SIN_Q12(0) and COS_Q12(0) fold to
 * gSinLut[0] and gSinLut[0x40] -- note gSinLut is DECLARED [0x40] in
 * hardware.h while COS_Q12 indexes 0x40 past it, which is the existing
 * convention here and not something this function introduces.
 *
 * COS_Q12(0) * 16 appears twice (pa and pd) and is CSEd into r8, which is what
 * forces r8 to be pushed; SIN_Q12(0) is likewise read once into r4 and used
 * for both the negated pb and the plain pc.
 *
 * Proc_End does NOT return -- the -1 arm falls straight through into the
 * SetObjAffine call in the ROM too, so there is no `return` after it. */
struct Unk39BDCProc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2e);
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ STRUCT_PAD(0x30, 0x32);
    /* 0x32 */ s16 unk32;
};

void SparkleZoom_Init(struct Unk39BDCProc *proc)
{
    proc->unk32 = AllocObjAffineSlot(proc);

    if (proc->unk32 == -1)
        Proc_End(proc);

    SetObjAffine(proc->unk32,
                 Div(COS_Q12(0) * 16, 1),
                 Div(-SIN_Q12(0) * 16, 1),
                 Div(SIN_Q12(0) * 16, 1),
                 Div(COS_Q12(0) * 16, 1));

    proc->unk2e = 0;
}
asm(".global sub_08039BDC\n.thumb_set sub_08039BDC, SparkleZoom_Init\n");
