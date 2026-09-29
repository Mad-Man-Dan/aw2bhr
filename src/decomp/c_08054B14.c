#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08054B14.
 * sub_08054B14 @ 0x08054B14
 */

/* Clear a 2 x 8 u16 block at gUnknown_03004580, then two u16 flags.
 *
 * WAVE 59 (W59-E) MEASUREMENT -- loop direction is NOT decidable here.
 * The ROM walks the store address DOWN (`strh; subs r0,#2`) with the counter
 * running 7 -> 0 (`movs r1,#7 ... subs r1,#1; cmp r1,#0; bge`). BOTH of these
 * spellings compile to the SAME 64 bytes, verified by exit status on one and
 * compile_probe on the other:
 *
 *   for (j = 7; j >= 0; j--)  arr[i*8 + j] = 0;      (the wave-36 draft)
 *   for (j = 0; j < 8;  j++)  arr[i*8 + j] = 0;      (this file)
 *
 * So a descending ADDRESS is no better evidence of a descending source loop
 * than a descending counter is: check_dbra_loop reverses the counter AND
 * re-bases the address giv to walk down with it. The ascending form is kept
 * because it is the more plausible original and needs no helper locals; the
 * wave-36 draft's explicit `o` and `next` binding locals are not load-bearing
 * either -- `i * 16` and the early `adds r2, r1, #1` fall out of allocation,
 * because r1 is reused as the inner counter. */
void ResetBattleAnimParams(void)
{
    int i;
    int j;

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 8; j++)
            *(u16 *)((u8 *)gUnknown_03004580 + i * 16 + j * 2) = 0;
    }

    gUnknown_0300450C = 0;
    gUnknown_03004520 = 0;
}
asm(".global sub_08054B14\n.thumb_set sub_08054B14, ResetBattleAnimParams\n");
