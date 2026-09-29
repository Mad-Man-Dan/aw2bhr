#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075F1C.
 * sub_08075F1C @ 0x08075F1C
 */

void PutIndexedTileBlock2x2(u16 *dst, int a)
{
    u16 v;

    v = a * 4 + 0x29;
    *dst = v;
    dst++;
    v++;
    *dst = v;
    dst += 0x1f;
    v++;
    *dst = v;
    v++;
    dst[1] = v;
}
asm(".global sub_08075F1C\n.thumb_set sub_08075F1C, PutIndexedTileBlock2x2\n");
