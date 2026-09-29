#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031638.
 * sub_08031638 @ 0x08031638
 */

#include "hardware.h"
#include "proc.h"

void LinkLobby_Loop(ProcPtr proc)
{
    int n;
    int i;

    n = 0;

    if (SioCountLinkedPlayers() > 1 && SioCountSendingPlayers() > 1 && SioAreAllLinkedPlayersStatus5() == 1
        && gUnknown_0849B018->unk06 == 0)
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 2, 2);
    else
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 1, 2);

    for (i = 0; i < 4; i++)
    {
        if (gUnknown_0849B018->unk16[i] > 0x3c)
            n++;
    }

    if (!SioIsConnectionAlive() || gUnknown_0849B018->unk1a > 0x3c || n != 0)
    {
        gUnknown_0849B018->unk04 = 7;
        Proc_Break(proc);
        return;
    }

    if (gUnknown_0849B018->unk0a[gUnknown_0849B018->unk06] == 2)
    {
        Proc_Break(proc);
        return;
    }

    if (gpKeySt->pressed & 2)
        PlayMusicOrSfx2(0x68);

    if (SioAreAllLinkedPlayersStatus5() == 1 && gUnknown_0849B018->unk06 == 0
        && (gpKeySt->pressed & 9))
    {
        gUnknown_0300410C = gUnknown_030040CC;

        gUnknown_0849B018->unk04 = 6;
        gUnknown_0849B018->unk1a = 0;

        for (i = 0; i < 4; i++)
            gUnknown_0849B018->unk16[i] = 0;

        gUnknown_03004400[0] = 0xff;
        LinkQueueCommand((u8 *)gUnknown_03004400);

        PlayMusicOrSfx2(0x71);
        Proc_EndEach(gUnknown_0849B1A0);

        Proc_Goto(proc, 1);
    }
    else if (LinkReceiveCommand((void *)gUnknown_03004400, 0) != -1
             && SioIsPlayerLinked(gUnknown_0849B018->unk06) == 1
             && gUnknown_03004400[0] == 0xff)
    {
        gUnknown_0300410C = gUnknown_030040CC;

        gUnknown_0849B018->unk04 = 6;
        gUnknown_0849B018->unk1a = 0;

        for (i = 0; i < 4; i++)
            gUnknown_0849B018->unk16[i] = 0;

        gUnknown_0849B060->unk02 = 2;

        PlayMusicOrSfx2(0x71);

        Proc_Goto(proc, 1);
    }
    else
    {
        LinkSendHelloPacket();
    }
}
asm(".global sub_08031638\n.thumb_set sub_08031638, LinkLobby_Loop\n");
