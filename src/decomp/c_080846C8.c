#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080846C8.
 * sub_080846C8 @ 0x080846C8, sub_080846DC @ 0x080846DC
 */

#include "proc.h"

void StartMainMenuCarousel(void)
{
    Proc_Start(ProcScr_MainMenuC1, PROC_TREE_3);
}
asm(".global sub_080846C8\n.thumb_set sub_080846C8, StartMainMenuCarousel\n");

int IsMainMenuCarouselRunning(void)
{
    return Proc_Find(ProcScr_MainMenuC1) != 0;
}
asm(".global sub_080846DC\n.thumb_set sub_080846DC, IsMainMenuCarouselRunning\n");
