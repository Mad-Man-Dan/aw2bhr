#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AEC4.
 * sub_0806AEC4 @ 0x0806AEC4
 */

/* Loads one gUnknown_0858178C row: four Decompress destinations and a palette
 * whose LENGTH is 0xe0 for row 7 and 0xc0 otherwise.
 *
 * Both arms read `gUnknown_0858178C[a].unk10`; the ROM's `a == 7` arm shows a
 * bare `+0x9c` with no index scaling because 7 * 0x14 + 0x10 == 0x9c and cse's
 * record_jump_equiv knows a == 7 there. That is a constant fold, not a
 * different expression, and writing the literal 7 in the source is what
 * produces it. */
void LoadIllustrationToBg0(int a)
{
    Decompress(gUnknown_0858178C[a].unk00, (void *)0x06000000);
    Decompress(gUnknown_0858178C[a].unk04, (void *)0x06001B00);
    Decompress(gUnknown_0858178C[a].unk08, (void *)0x06003600);
    Decompress(gUnknown_0858178C[a].unk0c, gBG0TilemapBuffer);

    if (a == 7)
        ApplyPaletteExt(gUnknown_0858178C[a].unk10, 0, 0xe0);
    else
        ApplyPaletteExt(gUnknown_0858178C[a].unk10, 0, 0xc0);
}
asm(".global sub_0806AEC4\n.thumb_set sub_0806AEC4, LoadIllustrationToBg0\n");
