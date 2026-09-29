#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014D20.
 * sub_08014D20 @ 0x08014D20
 */

/* Pixel width to tiles. The `bge; adds #7; asrs #3` bias sequence is a SIGNED
 * divide by 8, so GetStringPixelWidth's return is a signed word -- an unsigned one
 * would be a bare `lsrs #3`. The string argument is forwarded untouched. */
int GetStringWidthInTiles(const char *s)
{
    return (GetStringPixelWidth(s) + 6) / 8;
}
asm(".global sub_08014D20\n.thumb_set sub_08014D20, GetStringWidthInTiles\n");
