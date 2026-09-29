#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E01C.
 * sub_0803E01C @ 0x0803E01C
 */

void *AddInventionRecord(int a1, int a2, int a3, int a4, int a5, int a6)
{
    struct Unk02028360 *p;

    for (p = gUnknown_02028360; p->unk02_6 != 0; p++)
        ;
    p->unk00 = a1;
    p->unk01 = a2;
    p->unk02_0 = a3;
    p->unk02_3 = a4;
    p->unk02_6 = a5;
    p->unk04 = a6;
    return p;
}
asm(".global sub_0803E01C\n.thumb_set sub_0803E01C, AddInventionRecord\n");
