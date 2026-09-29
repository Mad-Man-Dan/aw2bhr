#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043FA8.
 * sub_08043FA8 @ 0x08043FA8
 */

void LoadCoMiniPortrait(int a, void *b, int c)
{
    sub_08011E54(gUnknown_084A0090[a].miniPortrait, b, 0x180);
    sub_08043AA0(a, c);
}
asm(".global sub_08043FA8\n.thumb_set sub_08043FA8, LoadCoMiniPortrait\n");
