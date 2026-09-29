#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017994.
 * sub_08017994 @ 0x08017994, sub_080179AC @ 0x080179AC
 */

/* gUnknown_08499588 is a `u16 *`, so the ROM's `adds r0, #0x4e` is element
 * 0x27 of it. */
void sub_08017994(void)
{
    ClearTilemapRect23x4(gUnknown_08499588 + 0x27);
    UploadEventTilemap();
}

/* Six arguments, the last two passed on the stack. r1 is left holding the 0 it
 * was given for the sixth argument, which is why the ROM sets up only r2 and r3
 * after the two `str`s -- the second argument needs no instruction of its
 * own. */
void sub_080179AC(void)
{
    FillTilemapRect(gUnknown_08499588, 0, 0, 0x1e, 6, 0);
}
