#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080326FC.
 * sub_080326FC @ 0x080326FC
 */

/* An if/else with the `ldrh` of unk00 DUPLICATED in both arms and only the
 * tail (`movs r2,#2; bl; strh`) shared -- that is agbcc cross-jumping two full
 * statements, not a `?:` selecting the message id, which would have loaded
 * unk00 once. */

void LinkShowMapTransferMessage(void)
{
    if (gUnknown_0849B060->unk09 == gUnknown_0849B018->unk06)
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 6, 2);
    else
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 7, 2);
}
asm(".global sub_080326FC\n.thumb_set sub_080326FC, LinkShowMapTransferMessage\n");
