#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F9C0.
 * sub_0801F9C0 @ 0x0801F9C0
 */

/* Paints a Manhattan-distance diamond of radius a3 centred on (a1, a2) into the
 * gUnknown_03003340 row-pointer plane, writing a4 to every cell -- the range
 * highlight c_08020354.c fills with 0 and then 0xff. The two loops are the two
 * halves of the diamond: the first runs y downward-inclusive from a2 while the
 * half-width r counts down from a3, the second runs y up from a2-1 with r from
 * a3-1, so both share one body.
 *
 * The row span is clamped on BOTH sides in the source, and the left clamp is
 * written as the compiler emits it: `x1 = a1 - r; w = r * 2 + 1;` then
 * `if (x1 < 0) { w += x1; x1 = 0; }`. Note x2 = x1 + w is the same value on both
 * arms (0 + (w + x1) == x1 + w), which is why the ROM computes `adds r2, r1, r0`
 * once after the merge rather than duplicating it -- do not "simplify" the
 * negative arm away, it is what produces `adds r0, r0, r1; movs r1, #0`.
 *
 * The map bounds are re-read from gUnknown_08499590 on every iteration, exactly
 * as c_0801F888.c's sub_0801F98C has them; the `mov sl, r6` that parks the pool
 * address is the loop optimiser's, not source.
 *
 * ORDER OF THE TWO INDUCTION VARIABLES IS THE WHOLE DIFFERENCE and it was worth
 * 10 bytes of 260: `r` must be initialised and stepped BEFORE `y` in both loops
 * (`for (r = a3, y = a2; ...; r--, y++)`). Written y-first the function is
 * size-exact at 96.2% with exactly three swapped instruction pairs -- the two
 * preheader `mov`s and the `subs r5,#1` / `adds r7,r4,#1` step pair.
 *
 * The 4th parameter was declared `int` in unknown-functions.h and is `u8`: the
 * prologue narrows r3 with `lsls #0x18 / lsrs #0x18`. Same correction wave 41
 * made to the twin MapSetInRangeSigned; see the note at the declaration.
 *
 * MATCHED (2 attempts). */
void MapSetInRange(u16 a1, u16 a2, u16 a3, u8 a4)
{
    int y;
    int r;
    int x;
    int x1;
    int x2;
    int w;

    for (r = a3, y = a2;
         y <= a2 + a3 && y < gMap->height;
         r--, y++)
    {
        x1 = a1 - r;
        w = r * 2 + 1;
        if (x1 < 0) {
            w += x1;
            x1 = 0;
        }
        x2 = x1 + w;
        if (x2 > gMap->width)
            x2 = gMap->width;
        for (x = x1; x < x2; x++)
            gUnknown_03003340[y][x] = a4;
    }

    for (r = a3 - 1, y = a2 - 1;
         y >= a2 - a3 && y >= 0;
         r--, y--)
    {
        x1 = a1 - r;
        w = r * 2 + 1;
        if (x1 < 0) {
            w += x1;
            x1 = 0;
        }
        x2 = x1 + w;
        if (x2 > gMap->width)
            x2 = gMap->width;
        for (x = x1; x < x2; x++)
            gUnknown_03003340[y][x] = a4;
    }
}
asm(".global sub_0801F9C0\n.thumb_set sub_0801F9C0, MapSetInRange\n");
