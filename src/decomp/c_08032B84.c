#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032B84.
 * sub_08032B84 @ 0x08032B84, sub_08032BA4 @ 0x08032BA4
 */

#include "hardware.h"

/* Half of a two-state HBlank/VCount ping-pong: this one parks BG0HOFS at 0 and
 * arms LinkMapPick_OnVCountMiddle for scanline 0x50; LinkMapPick_OnVCountMiddle restores the shadow and
 * arms this one back for scanline 0. The pool word is a FUNCTION address, so
 * the (int) cast is what makes it relocate against the symbol rather than
 * become a plain constant -- the same house convention as c_08039264.c's
 * (void *). */

void LinkMapPick_OnVCountTop(void)
{
    REG_BG0HOFS = 0;

    SetVCountCompareLine(0x50);
    SetVCountInterruptHandler((int)LinkMapPick_OnVCountMiddle);
}
asm(".global sub_08032B84\n.thumb_set sub_08032B84, LinkMapPick_OnVCountTop\n");

void LinkMapPick_OnVCountMiddle(void)
{
    REG_BG0HOFS = gUnknown_03001FF8;

    SetVCountCompareLine(0);
    SetVCountInterruptHandler((int)LinkMapPick_OnVCountTop);
}
asm(".global sub_08032BA4\n.thumb_set sub_08032BA4, LinkMapPick_OnVCountMiddle\n");
