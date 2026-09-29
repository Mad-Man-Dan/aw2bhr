#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D54C.
 * sub_0803D54C @ 0x0803D54C
 */

void SetMapIdToDesignSlot0(void)
{
    gPlaySt.mapID = 0xb4;
}
asm(".global sub_0803D54C\n.thumb_set sub_0803D54C, SetMapIdToDesignSlot0\n");
