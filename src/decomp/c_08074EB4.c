#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074EB4.
 * sub_08074EB4 @ 0x08074EB4
 */

#include "proc.h"
/* MATCHED (wave 38, W38-E), first attempt.
 *
 * Tears down the five sprites WorldMapScope_Init builds into proc->unk40[]. The proc
 * layout is that function's, unchanged: five `struct Unk0801C210 *` at +0x40.
 *
 * The ROM's `subs r4, #1; cmp r4, #0; bge` bottom is check_dbra_loop's rewrite
 * of the ASCENDING `for (i = 0; i <= 4; i++)`, not a descending source loop --
 * the `ldm r5!, {r0}` walk is upward, which settles the direction independently
 * of the counter. This is the wave-38 loop rule confirmed on a 28-byte function.
 */
struct Unk8074EB4Proc
{
    /* 0x00 */ u8 filler_00[0x40];
    /* 0x40 */ struct Unk0801C210 *unk40[5];
};

void WorldMapScope_OnEnd(struct Unk8074EB4Proc *proc)
{
    int i;

    for (i = 0; i <= 4; i++)
        AP_Delete(proc->unk40[i]);
}
asm(".global sub_08074EB4\n.thumb_set sub_08074EB4, WorldMapScope_OnEnd\n");
