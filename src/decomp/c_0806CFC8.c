#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806CFC8.
 * sub_0806CFC8 @ 0x0806CFC8
 */

void RulesScreenDrawUpArrow(int a1, int a2)
{
    if (gUnknown_03000610 == gGameClock - 1)
    {
        a1 = (gUnknown_03000614 + a1) / 2;
        a2 = (gUnknown_03000616 + a2) / 2;
    }

    DrawOamObject(0x43, a1 & 0x1ff, a2 & 0xff, 0, 0);

    gUnknown_03000610 = gGameClock;
    gUnknown_03000614 = a1;
    gUnknown_03000616 = a2;
}
asm(".global sub_0806CFC8\n.thumb_set sub_0806CFC8, RulesScreenDrawUpArrow\n");
