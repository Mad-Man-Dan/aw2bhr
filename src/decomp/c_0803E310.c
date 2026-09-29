#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E310.
 * sub_0803E310 @ 0x0803E310
 */

void AddFactoryInventionRecord(int a1, int a2, int a3, int a4, int a5, int a6)
{
    struct Unk02028360Pos pos;

    GetInventionOriginOffset(7, &pos);
    AddInventionRecordExt(a1 - pos.unk00, a2 - pos.unk02, 3, 4, 7, 0, a5, a6, 0, 0);
}
asm(".global sub_0803E310\n.thumb_set sub_0803E310, AddFactoryInventionRecord\n");
