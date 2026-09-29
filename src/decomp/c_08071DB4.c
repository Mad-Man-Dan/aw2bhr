#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071DB4.
 * sub_08071DB4 @ 0x08071DB4
 */

#include "hardware.h"
/* Opens blending inside both windows and inside neither, sets the blend
 * effect to 3 (darken), then commits a full target1/target2 layer mask and
 * seeds the proc's fade accumulator.
 *
 * The two masked inserts are raw halfword writes, not bitfield stores:
 * `ldrh` with a POOL mask is the raw tell, and 0x1F / 0x1F00 set five layer
 * bits at a time, which no single `.bits` assignment can produce. The
 * `orr #0x20` one line later is the same object through `.bits` -- `ldrb`
 * with a `movs` mask -- and the two spellings sitting a line apart is
 * exactly the case the hardware.h note describes.
 *
 * THE CAST IS LOAD-BEARING; `gUnknown_030030E0.raw` does NOT substitute for
 * it here. An isolated probe of the two spellings side by side emits
 * identical code, but in this function `.raw` costs +4 bytes: it materialises
 * the 0 written to gUnknown_03002020 into its own pseudo that stays live
 * across the insert (it comes back as a dead `orrs r1, r3`), which shuffles
 * the whole allocation and pushes an extra literal into the pool. Same
 * family as the wave-15 rule -- a probe agreeing about two spellings is
 * evidence about the PROBE's context, not a general fact. CoSelectConfirm_BeginFadeToWhite, two
 * functions along, matches with either spelling, so this is not a property
 * of the symbol.
 *
 * agbcc emits ONE ldrh/strh pair for both inserts: the second statement's
 * load is forwarded from the first's store because the object is not
 * volatile. The 0x1F00 arrives as `movs #0xf8; lsls #5` and the 0x100 at the
 * end as `adds r1, #0xf0` off the 0x10 still live in r1 from unk64. */
struct Unk08071DB4
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u16 unk66;
};

void FadeFromBlack_OnInitUnused(struct Unk08071DB4 *proc)
{
    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x10;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x1F;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x1F00;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk64 = 0x10;
    proc->unk66 = 0x100;
}
asm(".global sub_08071DB4\n.thumb_set sub_08071DB4, FadeFromBlack_OnInitUnused\n");
