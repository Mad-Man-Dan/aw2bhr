#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802323C.
 * sub_0802323C @ 0x0802323C
 */

/* Two s16 OUT parameters on the stack, which is the only reason this function
 * has an 8-byte frame: `add r3, sp, #4` is the first and
 * `mov r5, sp; adds r5, #6; str r5, [sp]` is the second going in as
 * EaseMapCursorDisplayToward's fifth argument. Both come back with `ldrsh`.
 *
 * The third parameter is `int` -- `adds r4, r2, #0` followed by a SEPARATE
 * `lsls #0x10; asrs #0x10` is the copy-then-narrow tell -- but BOTH calls get
 * the narrowed value, not the raw one. EaseMapCursorDisplayToward's third parameter is
 * declared s16 and supplies that conversion; DrawMapCursorSprite's third is declared
 * `int` by its own promoted definition (src/decomp/c_08043418.c), so the cast
 * has to be written here. Without it the ROM's `adds r2, r4, #0` at the second
 * call becomes a reload of the raw parameter and the register assignment for
 * the whole function shifts with it. */
void EaseMapCursorAndDraw(s16 a1, s16 a2, int a3)
{
    s16 x;
    s16 y;

    EaseMapCursorDisplayToward(a1, a2, a3, &x, &y);
    DrawMapCursorSprite(x, y, (s16)a3);
}
asm(".global sub_0802323C\n.thumb_set sub_0802323C, EaseMapCursorAndDraw\n");
