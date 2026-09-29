#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801FD9C.
 * sub_0801FD9C @ 0x0801FD9C
 */

/* MATCHED (wave 39, W39-B) -- 204/204 bytes, relocs match.
 *
 * Bleeds the value a1 outward by one cell across the gUnknown_03003340 screen:
 * for every cell that is non-negative and not already a1, each of its four
 * neighbours that is NEGATIVE and not off the edge of the map becomes a1.
 * The cells are SIGNED (`ldrsb` through the register-offset form, since ldrsb
 * has no immediate-offset encoding), so the u8 * rows are read through s8 *.
 *
 * The parameter is narrowed by a local copy rather than being declared u8, so
 * the `(int)` prototype in include/unknown-functions.h -- and the matched
 * c_08058744.c / c_0805DA84.c call sites -- are left alone.
 *
 * Both bounds are re-read from memory at every use, as c_0801F838.c describes;
 * the three row addresses (&gUnknown_03003340[y-1], [y] and [y+1]) are LICM
 * hoists into the inner loop's preheader, not source locals.
 *
 * THE ONE THING THAT CLOSED IT, after this function sat at 99.5% (203 of 204
 * bytes, size exact, ONE instruction differing in its register fields only)
 * across two waves and 19,274 permuter iterations:
 *
 *   original   adds r1, r2, r0     x + row      <- the RIGHT-neighbour block
 *   candidate  adds r1, r0, r2     row + x      <- and the other three blocks
 *
 * The earlier note ruled out source operand order on the grounds that C's
 * pointer_int_sum normalises `intop + ptrop` to pointer-first, so
 * `x + (s8 *)row` and `&((s8 *)row)[x]` compile identically. That is true, and
 * it is also the way out: DO THE ADDITION IN INTEGER SPACE. Casting the row
 * pointer to int makes both operands integers, pointer_int_sum never runs, and
 * gcc 2.95's fold only reorders commutative operands to put CONSTANTS last --
 * so a plain `x + (int)row` reaches expand with x still first and emits
 * `adds r1, r2, r0`. Only the right-neighbour block needs it; the other three
 * keep the ordinary pointer form. See the docs/agbcc-codegen.md section
 * "Integer-space address arithmetic is the operand-order lever pointer_int_sum
 * hides".
 *
 * Also refuted this wave, cheaply and negatively: giving the right-neighbour
 * block its OWN pointer local (`q` distinct from `p`) rather than reassigning
 * `p` changes nothing at all -- byte-identical, still row-first. The operand
 * order is not an allocation or live-range effect. */
void MapMarkHalo(int a1)
{
    int x;
    int y;
    u8 v;
    s8 *p;
    s8 *q;

    v = a1;
    for (y = 0; y < gMap->height; y++) {
        for (x = 0; x < gMap->width; x++) {
            p = (s8 *)gUnknown_03003340[y] + x;
            if (*p >= 0 && *p != v) {
                if (p[-1] < 0 && x != 0)
                    p[-1] = v;
                q = (s8 *)(x + (int)gUnknown_03003340[y]);
                if (q[1] < 0 && x != gMap->width - 1)
                    q[1] = v;
                p = (s8 *)gUnknown_03003340[y - 1] + x;
                if (*p < 0 && y != 0)
                    *p = v;
                p = (s8 *)gUnknown_03003340[y + 1] + x;
                if (*p < 0 && y != gMap->height - 1)
                    *p = v;
            }
        }
    }
}
asm(".global sub_0801FD9C\n.thumb_set sub_0801FD9C, MapMarkHalo\n");
