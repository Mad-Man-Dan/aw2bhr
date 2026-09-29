#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074C84.
 * sub_08074C84 @ 0x08074C84
 */

#include "proc.h"
struct Unk08614314
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s16 unk2c;
    /* 0x2e */ s16 unk2e;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
};

s32 StartWorldMapCameraPan(ProcPtr a1, s32 a2, s32 a3, u8 a4)
{
    struct Unk08614314 *proc;
    s32 x;
    s32 y;

    if (a4 == 0)
    {
        x = GetWorldMapCameraXKeepInBand(a2);
        y = GetWorldMapCameraYKeepInBand(a3);
    }
    else
    {
        x = GetWorldMapCameraXCentered(a2);
        y = GetWorldMapCameraYCentered(a3);
    }

    if ((x == gUnknown_0202FDFC.unk00 && y == gUnknown_0202FDFC.unk02)
        || Proc_Find(gUnknown_08614314) != NULL)
        return 0;

    if (a1 != NULL)
        proc = Proc_StartBlocking(gUnknown_08614314, a1);
    else
        proc = Proc_Start(gUnknown_08614314, PROC_TREE_3);

    proc->unk30 = gUnknown_0202FDFC.unk00;
    proc->unk32 = gUnknown_0202FDFC.unk02;
    proc->unk2c = x;
    proc->unk2e = y;
    proc->unk34 = a2;
    proc->unk36 = a3;

    return 1;
}
asm(".global sub_08074C84\n.thumb_set sub_08074C84, StartWorldMapCameraPan\n");
