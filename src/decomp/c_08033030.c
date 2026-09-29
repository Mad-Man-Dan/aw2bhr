#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033030.
 * sub_08033030 @ 0x08033030, sub_080330C0 @ 0x080330C0
 */

#include "proc.h"
#include "hardware.h"

void LinkMapPick_WaitForPress(ProcPtr proc)
{
    u8 v[0x34];

    if (gpKeySt->pressed & 2)
        PlayMusicOrSfx2(0x68);

    if ((gpKeySt->pressed & 9)
     && Proc_Find(gUnknown_0849B688) == NULL
     && Proc_Find(gUnknown_0849B670) == NULL)
    {
        if (gUnknown_0849B060->unk09 == gUnknown_0849B018->unk06
         || LoadDesignRoomName((u8)gUnknown_0849B060->unk04, v) != 1)
        {
            LockMainMenu();
            Proc_Goto(proc, 0xb);
        }
        else
        {
            Proc_Break(proc);
        }
    }
}
asm(".global sub_08033030\n.thumb_set sub_08033030, LinkMapPick_WaitForPress\n");

void LinkMapPick_Finish(ProcPtr proc)
{
    PlayMusicOrSfx2(0x71);

    if (GetMainMenuLock())
    {
        EndLinkMapPick();
        Proc_EndEach(gUnknown_0849B688);
        Proc_EndEach(gUnknown_0849B670);
        gUnknown_0849B060->unk08 = gUnknown_0849B060->unk04;
        gUnknown_03003F1C = gUnknown_030044C4 = 0;
    }
    else
    {
        Proc_Goto(proc, 0xa);
    }
}
asm(".global sub_080330C0\n.thumb_set sub_080330C0, LinkMapPick_Finish\n");
