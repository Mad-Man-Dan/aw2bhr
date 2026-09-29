#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073D98.
 * sub_08073D98 @ 0x08073D98
 */

#include "hardware.h"
#include "proc.h"
struct Unk73D98Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* The two halfwords are cleared in ONE chained assignment: the ROM materialises
 * BOTH addresses (gUnknown_0202FDE8 into the pool first, then
 * gUnknown_0202FDEA) and keeps them live across the two `strh`, storing to
 * 0x0202FDEA first. Two separate statements reuse one register for both
 * addresses and put the pool words the other way round -- 6 bytes. */
void SoundScope_Init(struct Unk73D98Proc *proc)
{
    m4aSoundInit();

    proc->unk58 = 0;

    gUnknown_0202FDE8 = gUnknown_0202FDEA = 0;

    gUnknown_030030A4.bits.win1_enable_bg0 = 0;
    gUnknown_030030A4.bits.win1_enable_bg1 = 0;
    gUnknown_030030A4.bits.win1_enable_bg2 = 1;
    gUnknown_030030A4.bits.win1_enable_bg3 = 1;
    gUnknown_030030A4.bits.win1_enable_obj = 1;

    gUnknown_030030DC.bits.win0_enable_bg0 = 0;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
}
asm(".global sub_08073D98\n.thumb_set sub_08073D98, SoundScope_Init\n");
