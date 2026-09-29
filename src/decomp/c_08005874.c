#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005874.
 * sub_08005874 @ 0x08005874, sub_08005964 @ 0x08005964
 */

#include "hardware.h"

/* sub_0800572C's shape with the first argument of the two forwarders changed
 * from 0 to 1 and a four-call tail. See work/sub_0800572C for the pool-word
 * note; the 0x0808D7C0 the listing names holds 0x0200B204. */
void sub_08005874(void)
{
    InitTilePool(0, (void *)(0x06000000 + (gUnknown_03002B6C.bits.chr_block << 14)), 0x2FC, 10);
    LoadTilePoolGraphic(9);
    if (LoadDesignRoomName(0, gDesignRoomName) != 1)
        sub_08004D74(1, 0);
    else
        sub_08004D90(1, 0, gDesignRoomName);
    if (LoadDesignRoomName(1, gDesignRoomName) != 1)
        sub_08004D74(1, 1);
    else
        sub_08004D90(1, 1, gDesignRoomName);
    if (LoadDesignRoomName(2, gDesignRoomName) != 1)
        sub_08004D74(1, 2);
    else
        sub_08004D90(1, 2, gDesignRoomName);
    gActiveMap->menuCursorX = 0x15;
    gActiveMap->menuCursorY = 0x30;
    DrawWindowBackgroundOnBg2(3, 2, 0xA, 4);
    PutTextScriptImmediate(4, 3, gBG0TilemapBuffer, gActiveMap->designName, 0x8000, 0);
    BG_EnableSyncBG0();
}

/* sub_080057EC's shape with a InitTextTileCache fade in front; see that draft for
 * why the slot id is read twice rather than bound to a local. */
void sub_08005964(void)
{
    InitTextTileCache(0x70);
    sub_08019F2C(gUnknown_08488514, 2, 6, 0,
                 (s8)gActiveMap->designSlot < 0 ? 0 : (s8)gActiveMap->designSlot);
    sub_08005874();
    gActiveMap->state = 4;
}
