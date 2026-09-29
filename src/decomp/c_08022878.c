#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022878.
 * sub_08022878 @ 0x08022878, sub_080228B8 @ 0x080228B8
 */

/* The stop half of BlinkRangeOverlayTile's blink: push the OFF frame unconditionally
 * through RegisterDataMove (the immediate path, where BlinkRangeOverlayTile uses the queued
 * one) and park a 2 in the proc.
 *
 * RegisterDataMove returns int and the result is discarded here, so nothing about
 * the return is settled from this call. */
struct Unk228B8Proc
{
    /* 00 */ u8 filler_00[0x20];
    /* 20 */ u16 unk20;
};

/* A two-frame blink: bit 0 of the frame counter picks which 0x80-byte blob goes
 * to 0x06003600. Both arms carry their OWN copy of the destination constant --
 * two pool words holding 0x06003600 -- because agbcc gives each basic block its
 * own literal; that is not evidence of two different addresses. */
void BlinkRangeOverlayTile(void)
{
    if (gGameClock & 1)
        CpuCopyAuto(gUnknown_08091B9C, (void *)0x06003600, 0x80);
    else
        CpuCopyAuto(gUnknown_0809181C, (void *)0x06003600, 0x80);
}
asm(".global sub_08022878\n.thumb_set sub_08022878, BlinkRangeOverlayTile\n");

void RangeOverlayScript_Init(struct Unk228B8Proc *proc)
{
    RegisterDataMove(gUnknown_0809181C, (void *)0x06003600, 0x80);
    proc->unk20 = 2;
}
asm(".global sub_080228B8\n.thumb_set sub_080228B8, RangeOverlayScript_Init\n");
