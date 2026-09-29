#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005154.
 * sub_08005154 @ 0x08005154, sub_0800517C @ 0x0800517C, sub_0800518C @ 0x0800518C
 */

void sub_08005154(void)
{
    CloseTopMenu();
    sub_080152EC(gUnknown_08487E8C, 0);
    gActiveMap->state = 7;
    RebuildMapUnitLayers2();
}

/* A bare `strb` through `adds rN, #0x9c`, past `strb`'s imm5 range -- which is
 * the only reason the address arithmetic is a separate instruction. */
void DesignRoomClearName(void)
{
    gActiveMap->designName[0] = 0;
}
asm(".global sub_0800517C\n.thumb_set sub_0800517C, DesignRoomClearName\n");

/* gBG0TilemapBuffer and gBG2TilemapBuffer are POINTER variables, so each
 * argument is `ldr rN, =sym; ldr rN, [rN]`. The 0x14 that goes to [sp] is CSEd
 * into r4 across both calls, which is what costs the function its `push {r4}`;
 * `movs r1,#0xd8; lsls r1,#2` is just the constant 0x360. */
void sub_0800518C(void)
{
    FillTilemapRect(gBG0TilemapBuffer, 0, 0xE, 0x1E, 0x14, 0);
    FillTilemapRect(gBG2TilemapBuffer, 0, 0xE, 0x1E, 0x14, 0x360);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
}
