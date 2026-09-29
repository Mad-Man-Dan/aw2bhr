#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078BDC.
 * sub_08078BDC @ 0x08078BDC, sub_08078C18 @ 0x08078C18
 */

#include "hardware.h"
#include "proc.h"
/* A blend FADE-IN step, and the twin of FadeInBrightness_Loop. The apparent double
 * indirection -- `ldr r2, =gUnknown_081D92D8; ldr r1, [r2]; ldrh r0, [r1]` --
 * is NOT a pointer variable: 0x081D92D8 is one of four consecutive
 * `-fforce-addr` address-constant words that all hold 0x03001FFC, so the
 * global is gUnknown_03001FFC, the volatile BLDY shadow, and 0x10 is its
 * maximum. Declaring a `u16 *` at that address instead compiles to three loads
 * and misses by 4 bytes. See the note in include/unknown-globals.h.
 *
 * `volatile` is what keeps the second `ldr r0, [r2]; ldrh` after the `if`
 * instead of reusing the value the increment already had. */

struct Unk8078BDC
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk_4c;
};

void FadeOutBrightness_Loop(struct Unk8078BDC *proc)
{
    if (gGameClock & 1)
        gUnknown_03001FFC++;

    if (gUnknown_03001FFC == 0x10)
    {
        proc->unk_4c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08078BDC\n.thumb_set sub_08078BDC, FadeOutBrightness_Loop\n");

void FadeInBrightness_Loop(struct Unk8078BDC *proc)
{
    if (gGameClock & 1)
        gUnknown_03001FFC--;

    if (gUnknown_03001FFC == 0)
    {
        proc->unk_4c = 0;
        Proc_Break(proc);
    }
}
asm(".global sub_08078C18\n.thumb_set sub_08078C18, FadeInBrightness_Loop\n");
