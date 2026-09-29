#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801113C.
 * sub_0801113C @ 0x0801113C, sub_0801117C @ 0x0801117C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* sub_080110A4's fade-DOWN twin, and the 0x08011 copy of FadeFromCommon_OnLoopUnused: clamp
 * at 0 and publish `>> 8`. The floor test is the bare sign test
 * `lsls #0x10; cmp #0; bgt` -- no `asrs`, because only the sign bit matters. */
struct Unk801113C
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};
/* Arms DMA0 from a five-field descriptor and then calls the descriptor's own
 * completion hook. The control halfword is disarmed first and written last,
 * which is the standard safe re-arm and is why REG_DMA0CNT_H appears twice.
 *
 * The named registers are the right spelling even though agbcc reaches DAD and
 * CNT_L as `adds r2, #4` off the SAD pool word rather than emitting fresh
 * pool entries -- the same CSE the hardware.h note on DMA1/DMA2 already
 * records, so a `vu16 *` cursor is not needed here either.
 *
 * `bl _call_via_r0` with the pointer in r0 is a NULLARY indirect call: r0 is
 * the first free scratch register only because nothing is being passed. */
struct Unk801117C
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ const void *unk2c;
    /* 0x30 */ void *unk30;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ void (*unk38)(void);
};

void FadeFromCommon_OnLoop(struct Unk801113C *proc)
{
    if (gUnknown_03001FFC == 0)
    {
        Proc_End(proc);
        return;
    }

    proc->unk66 -= proc->unk64;

    if (proc->unk66 <= 0)
        proc->unk66 = 0;

    gUnknown_03001FFC = proc->unk66 >> 8;
}

void FadeScreenLines_IDLE_0801117D(struct Unk801117C *dma)
{
    REG_DMA0CNT_H = 0;
    REG_DMA0SAD = (u32)dma->unk2c;
    REG_DMA0DAD = (u32)dma->unk30;
    REG_DMA0CNT_L = dma->unk34;
    REG_DMA0CNT_H = dma->unk36;

    dma->unk38();
}

asm(".global sub_0801113C\n.thumb_set sub_0801113C, FadeFromCommon_OnLoop\n"
    ".global sub_0801117C\n.thumb_set sub_0801117C, FadeScreenLines_IDLE_0801117D\n");
