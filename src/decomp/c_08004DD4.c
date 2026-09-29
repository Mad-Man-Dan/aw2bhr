#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004DD4.
 * sub_08004DD4 @ 0x08004DD4
 */

void sub_08004DD4(int a1, int a2, u8 * a3, int a4)
{
    int y;

    y = a1 == 0 ? 5 : 7;
    y += a2 * 2;
    FillTilemapRect(gBG0TilemapBuffer, a4 + 3, y, 9, 2, 0);
    PutTextScriptImmediate((s16)(a4 + 3), (s16)y, gBG0TilemapBuffer, a3, 0x8000, 0);
    BG_EnableSyncBG0();
}
