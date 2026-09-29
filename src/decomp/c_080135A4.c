#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080135A4.
 * sub_080135A4 @ 0x080135A4
 */

#include "hardware.h"

/* The palette-upload half of FlushPalette's pair. During forced blank the
 * shadow goes straight out with the CpuSet blitter; otherwise it is queued
 * once through RegisterDataMove and the dirty latch gUnknown_03000048 is raised so
 * FlushPalette does not queue it again. RegisterDataMove's s16 result is dropped.
 *
 * The two `movs`/`lsls` pairs are agbcc materialising PLTT (0x05000000) and
 * PLTT_SIZE (0x400) -- neither constant has low bits, so the shift form beats
 * a pool word, exactly as in FlushPalette.
 */
void EnablePaletteSync(void)
{
    if (gDispIo.disp_ct.forced_blank != 0)
    {
        CpuCopyAuto(gPal, (void *)PLTT, PLTT_SIZE);
    }
    else if (gUnknown_03000048 == 0)
    {
        RegisterDataMove(gPal, (void *)PLTT, PLTT_SIZE);
        gUnknown_03000048 = 1;
    }
}
asm(".global sub_080135A4\n.thumb_set sub_080135A4, EnablePaletteSync\n");
