#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E0F4.
 * sub_0803E0F4 @ 0x0803E0F4
 */

u8 GetInventionHpAt(int a1, int a2)
{
    struct Unk02028360 *p = FindInventionAt(a1, a2);

    if (p == NULL)
        return 0;
    return p->unk04;
}
asm(".global sub_0803E0F4\n.thumb_set sub_0803E0F4, GetInventionHpAt\n");
