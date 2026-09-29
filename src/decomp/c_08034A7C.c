#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034A7C.
 * sub_08034A7C @ 0x08034A7C
 */

void PutArmyNameBanner(int y, int b)
{
    char *s = (char *)gTextTable[gUnknown_08499CCC[b]];
    int x = GetCenteredTextX(s);

    PutCenteredAsciiStringSprites(y, s);

    DrawOamObject(b + 0x3d, x - 0x10, y - 4, 0, 0);
    DrawOamObject(b + 0x3d, x + sub_0808B6B0(s) * 8, y - 4, 0, 0);
}
asm(".global sub_08034A7C\n.thumb_set sub_08034A7C, PutArmyNameBanner\n");
