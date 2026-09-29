#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C0E8.
 * sub_0804C0E8 @ 0x0804C0E8
 */

/* Family F055 (data/families.json): `push {lr}; movs r0,#K; bl A; movs r0,#1;
 * bl B; pop {r0}; bx r0` -- 20 bytes, three members. `varies` covers index 1
 * (the first immediate) and the two `bl` targets; index 3 is absent, so the
 * `movs r0, #1` is byte-identical across all three.
 *
 * Two independent calls with their arguments materialised in front of each --
 * no nesting is possible, both callees have their r0 written by a `movs`.
 * `pop {r0}` is the void epilogue.
 */


/* Same one-callee-twice shape as StepPanelBounceBothSides, over sub_0804FF44. */

void sub_0804C0E8(void)
{
    sub_0804FF44(0);
    sub_0804FF44(1);
}
