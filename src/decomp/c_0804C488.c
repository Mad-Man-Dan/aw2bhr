#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C488.
 * sub_0804C488 @ 0x0804C488, sub_0804C498 @ 0x0804C498
 */

/* F086 -- `push {lr}; lsls #0x10; lsrs #0x10; bl` and nothing else.
 *
 * The narrowing is PROMOTE_MODE on a declared-narrow parameter: agbcc
 * zero-extends every sub-word parameter into its pseudo at entry. `int` with
 * an explicit `(u16)` cast at the call is byte-identical here and a probe
 * cannot separate the two, so the choice is made callee-side --
 * SpawnWholeFigure takes the u16 unit index the rest of that family takes,
 * and a wrapper that exists only to forward it takes the same thing.
 */
void SpawnWholeFigure2(u16 a)
{
    SpawnWholeFigure(a);
}
asm(".global sub_0804C488\n.thumb_set sub_0804C488, SpawnWholeFigure2\n");

/* F086 -- `push {lr}; lsls #0x10; lsrs #0x10; bl` and nothing else.
 *
 * The narrowing is PROMOTE_MODE on a declared-narrow parameter: agbcc
 * zero-extends every sub-word parameter into its pseudo at entry. `int` with
 * an explicit `(u16)` cast at the call is byte-identical here and a probe
 * cannot separate the two, so the choice is made callee-side --
 * SpawnWholeFigure takes the u16 unit index the rest of that family takes,
 * and a wrapper that exists only to forward it takes the same thing.
 */
void SpawnWholeFigure3(u16 a)
{
    SpawnWholeFigure(a);
}
asm(".global sub_0804C498\n.thumb_set sub_0804C498, SpawnWholeFigure3\n");
