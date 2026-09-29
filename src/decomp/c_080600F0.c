#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080600F0.
 * sub_080600F0 @ 0x080600F0
 */

void AiExecuteBuildUnit(void)
{
    BuyUnit(gUnknown_030046C0.unk02, gUnknown_030046C0.unk03, gUnknown_030046C0.unk01);
    gUnknown_030045D4 = 0xb;
}
asm(".global sub_080600F0\n.thumb_set sub_080600F0, AiExecuteBuildUnit\n");
