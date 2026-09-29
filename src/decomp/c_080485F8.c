#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080485F8.
 * sub_080485F8 @ 0x080485F8
 */

/* Re-entry into the map/menu view: reset one piece of state, repaint the
 * 7x0xf..0x17 window of *gBG0TilemapBuffer, flush, then stop the script that
 * sub_08014878 started and re-run the blob ShopScreen_StartMessage parked in unk850.
 *
 * Every result is discarded and the epilogue is `pop {r0}; bx r0`, the void
 * spelling. unk850's type comes from EndEventScript's `const u8 *`, the same
 * argument that types it in src/decomp/c_080485DC.c. */
void ShopScreen_EndMessage(void)
{
    SetChoiceResult(1);
    FillTilemapRect(gBG0TilemapBuffer, 7, 0xf, 0x17, 4, 0);
    BG_EnableSyncBG0();
    sub_0801537C(gUnknown_08489530);
    EndEventScript(gUnknown_084C30F8->unk850);
}
asm(".global sub_080485F8\n.thumb_set sub_080485F8, ShopScreen_EndMessage\n");
