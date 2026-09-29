#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043E3C.
 * sub_08043E3C @ 0x08043E3C
 */

void LoadCoFace(int a, void *b, int c)
{
    Decompress(gUnknown_084A0090[a % 24].face[a / 24], b);
    LoadCoPalette(a, c);
}
asm(".global sub_08043E3C\n.thumb_set sub_08043E3C, LoadCoFace\n");
