#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B9EC.
 * sub_0803B9EC @ 0x0803B9EC, sub_0803BA00 @ 0x0803BA00
 */

#include "proc.h"

void StartSoundRoom(void)
{
    Proc_Start(ProcScr_SoundRoom, PROC_TREE_3);
}
asm(".global sub_0803B9EC\n.thumb_set sub_0803B9EC, StartSoundRoom\n");

/* Same shape as sub_0803B8C4 (unk01 = 3) and StartCampaignAfterMap (unk01 = 1). */
void StartWarRoom(void)
{
    gPlaySt.gameMode = 2;
    Proc_Start(ProcScr_WarRoom, PROC_TREE_3);
}
asm(".global sub_0803BA00\n.thumb_set sub_0803BA00, StartWarRoom\n");
