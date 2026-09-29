#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BF74.
 * sub_0807BF74 @ 0x0807BF74
 */

#include "hardware.h"
#include "proc.h"
struct Proc7BF74
{
    u8 filler_00[0x58];
    int unk_58;
};

void MissionTitleName_UpdateSpin(ProcPtr proc)
{
    struct BgAffineSrcData src;

    ((struct Proc7BF74 *)proc)->unk_58 -= 0x100;

    src.texX = 0x5000;
    src.texY = 0x5000;
    src.scrX = 0x50;
    src.scrY = 0x50;
    src.sx = 0x100;
    src.sy = 0x100;
    src.alpha = ((struct Proc7BF74 *)proc)->unk_58;

    BgAffineSet(&src, (struct BgAffineDstData *)gUnknown_030024D0, 1);
}
asm(".global sub_0807BF74\n.thumb_set sub_0807BF74, MissionTitleName_UpdateSpin\n");
