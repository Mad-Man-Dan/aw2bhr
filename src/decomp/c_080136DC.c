#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080136DC.
 * sub_080136DC @ 0x080136DC
 */

/* Unpacks `h` palettes of 16 colours from `src` into the RGB shadow
 * gUnknown_0200B614 (three bytes per colour at `(row * 16 + i) * 3`, the same
 * layout LoadFadeComponentsFromPalette writes -- see src/decomp/c_08075904.c), and stamps `pal`
 * into gUnknown_0200B5F4 for each row touched.
 *
 * `pal` is SIGNED 8-bit and its sign bit is a flag: the ROM shares one
 * `lsls #0x18` between `lsrs #0x18` (the u8 value stored to gUnknown_0200B5F4)
 * and `asrs #0x1f` (`pal >> 7`, masked to 0x20 and added to every channel).
 * A `u8` parameter would need neither shift, an `s8` used directly only the
 * arithmetic one; both spellings together are what produces all three.
 *
 * `p` must be a LOCAL copy of `src` rather than the parameter incremented in
 * place: a modified parameter is copied to its register in the prologue, where
 * the ROM emits `adds r4, r0, #0` after the `pal` statements. */
void ColFadeDirect(u16 *src, u16 y, u16 h, s8 pal)
{
    u8 c;
    u8 base;
    u16 *p;
    u16 i;
    u16 j;

    c = pal;
    base = (pal >> 7) & 0x20;
    p = src;

    for (i = y; i < y + h; i++)
    {
        gUnknown_0200B5F4[i] = c;

        for (j = 0; j < 16; j++)
        {
            gUnknown_0200B614[(i * 16 + j) * 3] = base + (p[0] & 0x1f);
            gUnknown_0200B614[(i * 16 + j) * 3 + 1] = base + ((p[0] >> 5) & 0x1f);
            gUnknown_0200B614[(i * 16 + j) * 3 + 2] = base + ((p[0] >> 10) & 0x1f);
            p++;
        }
    }
}
asm(".global sub_080136DC\n.thumb_set sub_080136DC, ColFadeDirect\n");
