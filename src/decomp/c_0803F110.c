#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F110.
 * GetCannonFireTileGraphic @ 0x0803F110
 */

/* MATCHED. A LOOSE duplicate of the promoted GetCannonFireSpriteData -- same switch, two
 * different blobs. The return type is `u8 *` and not that function's
 * `const u16 *`: the note on gUnknown_0810A3E8 in include/unknown-globals.h
 * already recorded that this half of the accessor pair feeds Decompress rather
 * than APProc_Create, so the two halves are not the same kind of data. Byte-
 * neutral either way; the declaration follows the use. */
u8 *GetCannonFireTileGraphic(int a)
{
    switch (a)
    {
    case 3:
    default:
        return gUnknown_081095A4;

    case 4:
        return gUnknown_0810A6F0;
    }
}
asm(".global sub_0803F110\n.thumb_set sub_0803F110, GetCannonFireTileGraphic\n");
