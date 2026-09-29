#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080858C0.
 * sub_080858C0 @ 0x080858C0, sub_08085908 @ 0x08085908
 */

/* Decompresses a tile blob into *gBG2TilemapBuffer and biases every one of the
 * 0x400 halfwords by 0x360. Byte-identical duplicate of CoInfoScreen_LoadBg2UnitBonusBackdrop, which
 * names gUnknown_0823DF48 instead.
 *
 * The pointer global is re-read INSIDE the loop (`ldr r0,[r4]` every
 * iteration) -- that is what naming a non-const pointer global directly gives,
 * and binding it to a local would hoist the load out. The bias is written
 * `0x360 + x` and not `x + 0x360`: the constant is in the first operand of the
 * `adds`. */
void CoInfoScreen_LoadBg2Backdrop(void)
{
    int i;

    gUnknown_030030A0 = 0;
    Decompress(gUnknown_0823DE38, gBG2TilemapBuffer);

    for (i = 0; i <= 0x3ff; i++)
        gBG2TilemapBuffer[i] = 0x360 + gBG2TilemapBuffer[i];

    BG_EnableSyncBG2();
}
asm(".global sub_080858C0\n.thumb_set sub_080858C0, CoInfoScreen_LoadBg2Backdrop\n");

/* Byte-identical duplicate of CoInfoScreen_LoadBg2Backdrop -- only the source blob differs. */
void CoInfoScreen_LoadBg2UnitBonusBackdrop(void)
{
    int i;

    gUnknown_030030A0 = 0;
    Decompress(gUnknown_0823DF48, gBG2TilemapBuffer);

    for (i = 0; i <= 0x3ff; i++)
        gBG2TilemapBuffer[i] = 0x360 + gBG2TilemapBuffer[i];

    BG_EnableSyncBG2();
}
asm(".global sub_08085908\n.thumb_set sub_08085908, CoInfoScreen_LoadBg2UnitBonusBackdrop\n");
