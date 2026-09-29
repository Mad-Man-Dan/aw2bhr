#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005080.
 * sub_08005080 @ 0x08005080
 */

/* The dispatch is `cmp #1; beq; cmp #1; ble default; cmp #2; beq; b default`.
 * A two-case switch on {1,2} does NOT produce that -- agbcc omits the
 * low-bound test entirely and emits only the two `beq`s (measured three ways:
 * an int parameter, an int call result and a u8 member all give the short
 * ladder). The `ble` needs a THIRD case node at 0 whose body is empty: the
 * tree is then {0,1,2}, emit_case_nodes writes `cmp #1; bgt right; cmp #0;
 * beq join; b join` for the left half, and jump.c folds the dead `cmp #0` and
 * inverts the `bgt` into the `ble` seen here. */
void sub_08005080(void)
{
    int a;
    s16 t;

    a = sub_0800CB30(0, 0);
    SaveDesignRoomSlot(gActiveMap->designSlot, gActiveMap->designName, sub_0800C9E8());
    sub_0800CB30(1, a);
    t = 7;
    switch ((s8)gActiveMap->designSlot)
    {
    case 0:
        break;
    case 1:
        t = 9;
        break;
    case 2:
        t = 0xB;
        break;
    }
    FillTilemapRect(gBG0TilemapBuffer, 5, t, 9, 2, 0);
    PutTextScriptImmediate(5, t, gBG0TilemapBuffer, gActiveMap->designName, 0x8000, 0);
    BG_EnableSyncBG0();
    if (gActiveMap->flags & 0x100)
    {
        gActiveMap->flags &= 0xFEFF;
        gActiveMap->designName[0] = 0;
    }
    gActiveMap->flags &= 0xEFFF;
}
