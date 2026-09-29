#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043834.
 * sub_08043834 @ 0x08043834, DrawCoPowerLabel @ 0x08043898, sub_080438FC @ 0x080438FC
 */

void LoadCoPanelGraphics(int a)
{
    u16 (*tbl)[16];
    int i;

    gUnknown_030005D0 = 0;
    RegisterDataMove(gUnknown_08102824, (void *)0x06010000, 0x740);
    tbl = gUnknown_08104264;
    i = gPlayers[a].teamColor - 1;
    ApplyPaletteExt(tbl[i], 0x2e0, 0x20);
    RegisterDataMove(gUnknown_081259CC, (void *)0x06010840, 0xc0);
}
asm(".global sub_08043834\n.thumb_set sub_08043834, LoadCoPanelGraphics\n");

void DrawCoPowerLabel(int x, int y, int i)
{
    switch (gPlayers[i].coMode)
    {
    case 1:
    default:
        PutSprite(0, x, y + 0x18, gUnknown_084A0052, 0x702f);
        break;
    case 2:
        PutSprite(0, x, y + 0x18, gUnknown_084A005A, 0x7033);
        break;
    }
}

asm(".global sub_08043898\n.thumb_set sub_08043898, DrawCoPowerLabel\n");

void DrawCoPowerStarBar(int a, int b, int c)
{
    int va;
    int vy;
    int vu;
    int vt;
    int lo;
    int hi;
    int m;

    vu = gPlayers[c].coCharge;
    lo = GetCoPowerCost(c);
    hi = GetSuperCoPowerCost(c) - lo;
    m = GetCoPowerStarCost(c);
    vt = gGameClock;
    va = a;
    vy = b + 0x18;

    DrawCoPowerStarBarSegment(&va, &vy, &vu, m, lo, 0, vu >= lo, &vt);
    DrawCoPowerStarBarSegment(&va, &vy, &vu, m, hi, 1, vu >= hi, &vt);
}
asm(".global sub_080438FC\n.thumb_set sub_080438FC, DrawCoPowerStarBar\n");
