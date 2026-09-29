#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BED8.
 * sub_0807BED8 @ 0x0807BED8
 */

#include "proc.h"
#include "hardware.h"
struct Unk807BED8
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x4];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x20];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

void MissionTitleName_UpdateZoom(ProcPtr procv)
{
    struct Unk807BED8 *proc = procv;
    struct BgAffineSrcData src;

    proc->unk34++;

    proc->unk2c = Interpolate(4, 0x1E0, 0x50, proc->unk34, 0x32);
    proc->unk5c = Interpolate(4, 0x6A, 0x100, proc->unk34, 0x32);
    proc->unk58 -= Interpolate(5, 0x800, 0x200, proc->unk34, 0x32);

    if (DivRem(proc->unk34, 6) == 0)
        gUnknown_03002020--;

    src.texX = 0x5000;
    src.texY = 0x5000;
    src.scrX = proc->unk2c;
    src.scrY = 0x50;
    src.sx = proc->unk5c;
    src.sy = proc->unk5c;
    src.alpha = proc->unk58;

    BgAffineSet(&src, (struct BgAffineDstData *)gUnknown_030024D0, 1);
}
asm(".global sub_0807BED8\n.thumb_set sub_0807BED8, MissionTitleName_UpdateZoom\n");
