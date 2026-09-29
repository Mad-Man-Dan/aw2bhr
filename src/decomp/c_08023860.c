#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023860.
 * sub_08023860 @ 0x08023860
 */

/* Recomputes the six BG scroll shadows from the map header's camera pixel
 * position minus its 16x tile origin; the first pair is only refreshed while
 * the gUnknown_08499B4C script is live.
 *
 * The two locals are required: every destination global is a distinct store
 * that invalidates cse.c's memory table (four of the six are volatile), so
 * without a pseudo the +4 / +6 reads are reloaded in all six statements while
 * the ROM reloads only +0xc / +0xe.
 *
 * The binding load sits AFTER the product because the assignment is not the
 * subtraction's left operand: written `(x = ...) - product` the MODIFY_EXPR's
 * side effect pins it first and the load lands one instruction early (the
 * 83.3% shape parked by waves 33 and 35). Spelling the sum with the negated
 * product puts the assignment second, where expand_expr emits it after the
 * multiply. */

void UpdateMapBgScroll(void)
{
    int x;
    int y;

    if (FindSlotScript((s32)gUnknown_08499B4C) != -1)
    {
        gUnknown_03001FF8 = (u16)gMap->scrollX - gMap->camX * 16;
        gUnknown_03001418 = (u16)gMap->scrollY - gMap->camY * 16;
    }

    gUnknown_03002B34 = -(gMap->camX * 16) + (x = (u16)gMap->scrollX);
    gUnknown_03002F18 = -(gMap->camY * 16) + (y = (u16)gMap->scrollY) + 3;
    gUnknown_030030A0 = x - gMap->camX * 16;
    gUnknown_03001400 = y - gMap->camY * 16;
    gUnknown_0300200C = x - gMap->camX * 16;
    gUnknown_03002000 = y - gMap->camY * 16;
}
asm(".global sub_08023860\n.thumb_set sub_08023860, UpdateMapBgScroll\n");
