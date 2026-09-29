#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080413B4.
 * sub_080413B4 @ 0x080413B4
 */

/* Byte-identical duplicate of PushInventionFireEntry -- same array, different index
 * counter (gUnknown_030040A8). */
void AddAttackTarget(int a, int b, int c, int d)
{
    struct Unk03003338 *p;

    p = gUnknown_03003338;
    p[gUnknown_030040A8].unk04 = a;
    p[gUnknown_030040A8].unk06 = b;
    p[gUnknown_030040A8].unk00 = c;
    p[gUnknown_030040A8].unk02 = d;
    gUnknown_030040A8++;
}
asm(".global sub_080413B4\n.thumb_set sub_080413B4, AddAttackTarget\n");
