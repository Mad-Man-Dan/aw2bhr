#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F524.
 * sub_0807F524 @ 0x0807F524, sub_0807F538 @ 0x0807F538, sub_0807F550 @ 0x0807F550, sub_0807F564 @ 0x0807F564
 */

#include "proc.h"

void StartWarRoomMapSelected(void)
{
    Proc_Start(gUnknown_086165C0, PROC_TREE_3);
}
asm(".global sub_0807F524\n.thumb_set sub_0807F524, StartWarRoomMapSelected\n");

int IsWarRoomMapSelectedRunning(void)
{
    return Proc_Find(gUnknown_086165C0) != 0;
}
asm(".global sub_0807F538\n.thumb_set sub_0807F538, IsWarRoomMapSelectedRunning\n");

void sub_0807F550(void)
{
    Proc_Start(gUnknown_086166C8, PROC_TREE_3);
}

int sub_0807F564(void)
{
    return Proc_Find(gUnknown_086166C8) != 0;
}
