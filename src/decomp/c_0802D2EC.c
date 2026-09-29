#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D2EC.
 * sub_0802D2EC @ 0x0802D2EC
 */

#include "hardware.h"

/* Reloads the BG char block InitTilePool owns and then replays every entry of
 * the 0xFF-terminated gUnknown_0849AAA8 list through LoadTilePoolGraphic. The
 * destination expression is c_0800572C.c's and c_08005874.c's verbatim -- only
 * the 0x1b4 length differs from those two.
 *
 * The count loop is a plain `while` that agbcc inverts, so the pre-test reads
 * element 0 with no index and the cursor is a `u8` (`lsls/lsrs #0x18` each
 * iteration). */
void sub_0802D2EC(void)
{
    u8 i;

    InitTilePool(0, (void *)(0x06000000 + (gUnknown_03002B6C.bits.chr_block << 14)), 0x1b4, 10);

    i = 0;
    while (gUnknown_0849AAA8[i] != 0xff)
    {
        LoadTilePoolGraphic(gUnknown_0849AAA8[i]);
        i++;
    }
}
