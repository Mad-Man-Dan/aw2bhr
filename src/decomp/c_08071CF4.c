#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071CF4.
 * sub_08071CF4 @ 0x08071CF4
 */

#include "hardware.h"
/* Sibling of FadeFromBlack_OnInitUnused -- the same window/blend opening, but it zeroes
 * BLDY instead of setting it to 0x10, commits only the target1 half of the
 * layer mask, and seeds the proc accumulator with 0 rather than 0x100.
 * One fewer masked insert is the whole 16-byte difference.
 *
 * The `*(u16 *)&` cast on gUnknown_030030E0 is load-bearing for the same
 * reason as in FadeFromBlack_OnInitUnused: spelling it `.raw` keeps the 0 alive in its own
 * callee-saved pseudo (r5 instead of the r3 the ROM rematerialises at the
 * store), which costs the extra `push {r5}`. Size stays 124 either way, so
 * this one is a pure allocation difference and a size check would miss it. */
struct Unk08071CF4
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u16 unk66;
};

void FadeToBlack_OnInitUnused(struct Unk08071CF4 *proc)
{
    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk64 = 0x10;
    proc->unk66 = 0;
}
asm(".global sub_08071CF4\n.thumb_set sub_08071CF4, FadeToBlack_OnInitUnused\n");
