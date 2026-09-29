#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080859E8.
 * sub_080859E8 @ 0x080859E8, sub_08085A38 @ 0x08085A38
 */

#include "proc.h"

void CoInfoPopup_SlideInLoop(ProcPtr proc)
{
    DrawOamObject(0x92,
                 Interpolate(4, 0xF0, 0x38, ((s16 *)proc)[0x32]++, 8) & 0x1FF,
                 0x38, 0, 0);

    if (((s16 *)proc)[0x32] == 8)
    {
        ((s16 *)proc)[0x32] = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_080859E8\n.thumb_set sub_080859E8, CoInfoPopup_SlideInLoop\n");

void CoInfoPopup_SlideOutLoop(ProcPtr proc)
{
    DrawOamObject(0x92,
                 Interpolate(1, 0x38, -0x80, ((s16 *)proc)[0x32]++, 8) & 0x1FF,
                 0x38, 0, 0);

    if (((s16 *)proc)[0x32] == 8)
    {
        ((s16 *)proc)[0x32] = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08085A38\n.thumb_set sub_08085A38, CoInfoPopup_SlideOutLoop\n");
