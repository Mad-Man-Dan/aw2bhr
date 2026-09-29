#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E560.
 * sub_0803E560 @ 0x0803E560
 */

void PushInventionFireEntry(u16 a, u16 b, u16 c, u16 d)
{
    struct Unk03003338 *p;

    p = gUnknown_03003338;
    p[gUnknown_03003F40].unk04 = a;
    p[gUnknown_03003F40].unk06 = b;
    p[gUnknown_03003F40].unk00 = c;
    p[gUnknown_03003F40].unk02 = d;
    gUnknown_03003F40++;
}
asm(".global sub_0803E560\n.thumb_set sub_0803E560, PushInventionFireEntry\n");
