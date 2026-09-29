#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BAFC.
 * sub_0802BAFC @ 0x0802BAFC
 */

/* Draws a two-digit number: the units digit at x, the tens digit seven pixels
 * left, and the tens sprite is skipped when it is zero.
 *
 * `(s16)a3` must be spelled at both uses rather than bound to a local first.
 * As `n = (s16)a3;` its shift pair is emitted in the prologue, where the ROM
 * emits it after the 0x1ff mask, the 0x400 or and the gUnknown_0849A3B8 pool
 * load -- CSE shares the value either way, but only the inline form puts it
 * where the first use is. */
void DrawInfoPanelTwoDigitNumber(u16 a1, u16 a2, int a3)
{
    int d;

    PutOamHi(a1 & 0x1ff, a2 | 0x400, gUnknown_0849A3B8,
                 (s16)((s16)a3 % 10) + 0x10);
    d = (s16)((s16)a3 / 10);

    if (d != 0)
        PutOamHi((a1 - 7) & 0x1ff, a2 | 0x400, gUnknown_0849A3B8, d + 0x10);
}
asm(".global sub_0802BAFC\n.thumb_set sub_0802BAFC, DrawInfoPanelTwoDigitNumber\n");
