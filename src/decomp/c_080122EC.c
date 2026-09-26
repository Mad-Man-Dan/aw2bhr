#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080122EC.
 * sub_080122EC @ 0x080122EC
 */

#include "hardware.h"

void sub_080122EC(void)
{
    gUnknown_03001FF8 = 0;
    gUnknown_03001418 = 0;
    gUnknown_03002B34 = 0;
    gUnknown_03002F18 = 0;
    gUnknown_030030A0 = 0;
    gUnknown_03001400 = 0;
    gUnknown_0300200C = 0;
    gUnknown_03002000 = 0;
    gUnknown_030030C4 = 0;
    /* Storing through the union member instead would make agbcc re-materialise
     * the zero -- one extra `mov r1, #0` at the type change, which is where
     * this function grew by four bytes the first time. The ROM keeps the same
     * constant in r1 across all thirteen stores, so stay on a u16 lvalue.
     * sub_08012358 and sub_0801237C do not need this: their shadow stores are
     * not in the middle of a run of plain u16 ones. */
    *(u16 *)&gUnknown_03002B6C = 0;
    *(u16 *)&gUnknown_03001FE8 = 0;
    *(u16 *)&gUnknown_030030B4 = 0;
    *(u16 *)&gUnknown_0300251C = 0;
}
