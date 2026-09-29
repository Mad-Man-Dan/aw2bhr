#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080187C8.
 * sub_080187C8 @ 0x080187C8, sub_08018800 @ 0x08018800
 */

/* EventOp_DisableMiniPanel's twin, differing only in the constant stored to
 * gPlaySt.dispMiniPanel. */
bool8 EventOp_EnableMiniPanel(s16 a)
{
    gPlaySt.dispMiniPanel = 1;
    ResetDisplayEffects();
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_080187C8\n.thumb_set sub_080187C8, EventOp_EnableMiniPanel\n");

bool8 EventOp_DisableMiniPanel(s16 a)
{
    gPlaySt.dispMiniPanel = 0;
    ResetDisplayEffects();
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018800\n.thumb_set sub_08018800, EventOp_DisableMiniPanel\n");
