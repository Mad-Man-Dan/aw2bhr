#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C0CC.
 * sub_0802C0CC @ 0x0802C0CC, sub_0802C0D8 @ 0x0802C0D8
 */

/* A selector wrapper: StartSubmarineDiveEffect takes an `int` (bare `adds r6, r0, #0`
 * across four calls) and this pins it to 0. `pop {r0}; bx r0`, so void.
 */

void StartSubmarineDiveEffectDive(void)
{
    StartSubmarineDiveEffect(0);
}
asm(".global sub_0802C0CC\n.thumb_set sub_0802C0CC, StartSubmarineDiveEffectDive\n");

/* A selector wrapper: StartSubmarineDiveEffect takes an `int` (bare `adds r6, r0, #0`
 * across four calls) and this pins it to 1. `pop {r0}; bx r0`, so void.
 */

void StartSubmarineDiveEffectRise(void)
{
    StartSubmarineDiveEffect(1);
}
asm(".global sub_0802C0D8\n.thumb_set sub_0802C0D8, StartSubmarineDiveEffectRise\n");
