#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065200.
 * sub_08065200 @ 0x08065200
 */

int GetArmyColumnX(int a)
{
    int n = gUnknown_08580934->unk08;
    int q = (0xf0 - n * 0x32) / (n + 1);
    int x = q;
    int i;

    for (i = 0; i < a; i++) {
        x += 0x32;
        x += q;
    }

    return x;
}
asm(".global sub_08065200\n.thumb_set sub_08065200, GetArmyColumnX\n");
