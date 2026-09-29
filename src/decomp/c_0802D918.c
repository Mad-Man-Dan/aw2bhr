#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D918.
 * sub_0802D918 @ 0x0802D918
 */

void DeploymentScreen_Init(void)
{
    struct Unk03001470 *proc;

    RebuildMapUnitLayers2();
    SaveMapCursorPosition();

    gUnknown_03001418 = gUnknown_03001FF8 = 0;

    proc = &gUnknown_03001470[gUnknown_03001FBC];
    proc->unk1e = 0;
    proc->unk20 = 0;
    proc->unk22 = gUnknown_0300055A - 1;

    sub_0802D7B0();
    DrawWindowBackgroundOnBg2(1, 4, 0xf, 0x10);
    DrawDeploymentList(0);
    SetMapCursorDisplayPosition(8, 0x28);
    PlayMusicOrSfx2(0x65);
    StartDeploymentUnitInfo(gUnknown_02023830[0]);
    SetInfoBoxMode(1);
}
asm(".global sub_0802D918\n.thumb_set sub_0802D918, DeploymentScreen_Init\n");
