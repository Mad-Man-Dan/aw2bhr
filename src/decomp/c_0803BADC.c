#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BADC.
 * sub_0803BADC @ 0x0803BADC
 */

#include "proc.h"

/* Same shape as sub_0803B8C4 (unk01 = 3) and StartWarRoom (unk01 = 2). */
void StartCampaignAfterMap(void)
{
    gPlaySt.gameMode = 1;
    Proc_Start(gUnknown_0849EBBC, PROC_TREE_3);
}
asm(".global sub_0803BADC\n.thumb_set sub_0803BADC, StartCampaignAfterMap\n");
