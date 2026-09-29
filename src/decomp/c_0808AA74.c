#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AA74.
 * sub_0808AA74 @ 0x0808AA74, sub_0808AA88 @ 0x0808AA88
 */

#include "proc.h"

void StartCampaignIntro(void)
{
    Proc_Start(ProcScr_CampaignIntro, PROC_TREE_3);
}
asm(".global sub_0808AA74\n.thumb_set sub_0808AA74, StartCampaignIntro\n");

int IsCampaignIntroRunning(void)
{
    return Proc_Find(ProcScr_CampaignIntro) != 0;
}
asm(".global sub_0808AA88\n.thumb_set sub_0808AA88, IsCampaignIntroRunning\n");
