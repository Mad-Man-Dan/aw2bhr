#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C480.
 * sub_0802C480 @ 0x0802C480
 */

/* Not a Proc: +0x20 is proc_prev, inside PROC_HEADER's 0x29 bytes, and
 * src/proc.c pins that layout. The whole prefix is filler.
 */
struct Unk2C480Proc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u32 unk20;
};

void MinimapScreen_OnEnd(struct Unk2C480Proc *proc)
{
    u32 v = proc->unk20;

    if (v != *(u32 *)&gMap->scrollX)
    {
        gUnknown_030033E4.unk00 = (gMap->scrollX >> 4) + 7;
        gUnknown_030033E4.unk02 = (gMap->scrollY >> 4) + 5;
    }

    gUnknown_03003334 = 0;
}
asm(".global sub_0802C480\n.thumb_set sub_0802C480, MinimapScreen_OnEnd\n");
