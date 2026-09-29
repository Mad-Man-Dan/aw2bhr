#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085AC8.
 * sub_08085AC8 @ 0x08085AC8, sub_08085ADC @ 0x08085ADC
 */

#include "proc.h"

void LaunchCoInfoScreen(void)
{
    Proc_Start(gUnknown_08616B74, PROC_TREE_3);
}
asm(".global sub_08085AC8\n.thumb_set sub_08085AC8, LaunchCoInfoScreen\n");

int IsCoInfoScreenRunning(void)
{
    return Proc_Find(gUnknown_08616B74) != 0;
}
asm(".global sub_08085ADC\n.thumb_set sub_08085ADC, IsCoInfoScreenRunning\n");
