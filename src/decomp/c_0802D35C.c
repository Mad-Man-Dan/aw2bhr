#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D35C.
 * sub_0802D35C @ 0x0802D35C, sub_0802D3B0 @ 0x0802D3B0
 */

void ShowOptionHelpText(int a1)
{
    u16 x;
    int v;

    x = a1;

    v = GetOptionHelpWindowX();
    DrawWindowBackgroundOnBg2(v, 0xe, 0xd, 6);
    sub_0801537C(gUnknown_08489568);
    sub_080146D4((s16)(v + 1), 0xf, gBG0TilemapBuffer, x, 0x8000, 0x100);
}
asm(".global sub_0802D35C\n.thumb_set sub_0802D35C, ShowOptionHelpText\n");

void ClearOptionHelpWindow(void)
{
    int v;

    v = GetOptionHelpWindowX();
    sub_0801537C(gUnknown_08489568);
    FillTilemapRect(gBG0TilemapBuffer, v, 0xe, 0xd, 6, 0);
    FillTilemapRect(gBG2TilemapBuffer, v, 0xe, 0xd, 6, 0x360);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
}
asm(".global sub_0802D3B0\n.thumb_set sub_0802D3B0, ClearOptionHelpWindow\n");
