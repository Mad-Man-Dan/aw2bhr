#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804365C.
 * DrawDaysRemaining @ 0x0804365C, DrawArmyCoPanel @ 0x080436DC
 */

/* `t -= gUnknown_03004080 - 1;` written in ONE expression reassociates to
 * `(t + 1) - g`, which is not the ROM; the statement boundary is what stops
 * agbcc's fold() (W35-A). The two `ldr rA,=0x1FF; adds rB,rA,#0; ands rC,rB`
 * triplets that used to be one instruction short in each arm are DrawSpriteNumberFont2's
 * u16 parameters -- see the retyped declaration in unknown-functions.h. */
void DrawDaysRemaining(int x, int y)
{
    int t = GetMapTurnLimit();
    int n;

    if (t == 0)
        return;

    n = gUnknown_03004080 - 1;
    t -= n;
    if (t > 0x63)
        return;

    if (x <= 0x77)
        x += 0x55;

    PutSprite(0, x, y, gUnknown_084A0024, 0x1042);

    if (t > 9)
        DrawSpriteNumberFont2((x - 0xa) & 0x1ff, y, t);
    else
        DrawSpriteNumberFont2((x - 0xf) & 0x1ff, y, t);
}

asm(".global sub_0804365C\n.thumb_set sub_0804365C, DrawDaysRemaining\n");

void DrawArmyCoPanel(int x, int y, int pid)
{
    int off;

    gUnknown_030005D0 = pid;

    if (gPlaySt.fog != 0 && (gPlayers[pid].turnState & 2) == 0)
        PutAsciiStringSprites((x + 0x34) & 0x1ff, y + 3, gUnknown_0809136C);
    else
        DrawSpriteNumberFont2((x + 0x34) & 0x1ff, y + 3, gPlayers[pid].funds);

    PutSprite(0, x, y, gUnknown_084A0032, 0x7000);
    PutSprite(0, x, y, gUnknown_084A003A, 0xe03a);

    LoadCoPalette(gPlayers[pid].co, 0x1e);
    off = ((gPlayers[pid].co * 8) & 0x3ff) * 0x20;
    RegisterDataMove(gUnknown_08102F64 + off, (void *)0x06010740, 0x100);

    if (gPlaySt.coPowersEnabled != 0)
    {
        if (gPlayers[pid].coMode != 0)
            DrawCoPowerLabel(x, y, pid);
        else
            DrawCoPowerStarBar(x, y, pid);
    }

    if (AdvanceCoPowerReadyAnnouncement(pid))
    {
        if ((u8)IsSuperCoPowerReady(pid))
            PlayMusicOrSfx2(0x1e0);
        else
            PlayMusicOrSfx2(0x75);
    }
}
asm(".global sub_080436DC\n.thumb_set sub_080436DC, DrawArmyCoPanel\n");
