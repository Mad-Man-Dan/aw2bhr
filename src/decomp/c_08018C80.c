#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018C80.
 * sub_08018C80 @ 0x08018C80
 */

bool8 EventOp_SetArmyCo(s16 a)
{
    u8 i = gUnknown_0200C528[a].unk04->unk08;

    gPlayers[i].co = gUnknown_0200C528[a].unk04->unk0a;
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018C80\n.thumb_set sub_08018C80, EventOp_SetArmyCo\n");
