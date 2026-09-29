#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013580.
 * sub_08013580 @ 0x08013580
 */

#include "hardware.h"

/* Uploads the whole gPal shadow to palette RAM when the dirty latch is set.
 * The two `movs`/`lsls` pairs are agbcc materialising 0x05000000 (PLTT) and
 * 0x400 (PLTT_SIZE) -- neither constant has low bits, so the shift form is
 * cheaper than a pool word. */
void FlushPalette(void)
{
    if (gUnknown_03000048 == 1)
        CpuCopyAuto(gPal, (void *)PLTT, PLTT_SIZE);
}
asm(".global sub_08013580\n.thumb_set sub_08013580, FlushPalette\n");
