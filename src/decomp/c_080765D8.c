#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080765D8.
 * sub_080765D8 @ 0x080765D8
 */

#include "proc.h"
/* c_0807662C.c's sibling: the same two OAM blobs drawn at the same relative
 * offsets and the same CycleWorldMapCalloutColor tail, but with the affine block gone and
 * the +0x64 flag as the break condition instead of a frame counter. The single
 * `movs r5, #0` serves all three zero operands -- both PutSprite fifth
 * arguments and the +0x5c store -- which is ordinary CSE, not a shared local. */
struct Unk80765D8
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x28];
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
    /* 0x64 */ s16 unk64;
};

void WorldMapCallout_HoldLoop(struct Unk80765D8 *proc)
{
    PutSprite(1, proc->unk2c, proc->unk30, gUnknown_086144C0, 0);
    PutSpriteExt(1, proc->unk2c + proc->unk60, proc->unk30 + 0x10,
                 gUnknown_086144D4, 0);
    CycleWorldMapCalloutColor();

    if (proc->unk64 != 0)
    {
        proc->unk5c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_080765D8\n.thumb_set sub_080765D8, WorldMapCallout_HoldLoop\n");
