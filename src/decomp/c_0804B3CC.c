#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B3CC.
 * sub_0804B3CC @ 0x0804B3CC
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


/* Both `bl`s go to the SAME function here -- `varies` gives StepPanelBounce at
 * both index 2 and index 4 -- so this is one routine driven over its two
 * arguments, 0 then 1, and not two unrelated calls. The family is defined by
 * shape and not by callee, which is why that had to be read off the member
 * rather than carried over from the representative.
 */

void StepPanelBounceBothSides(void)
{
    StepPanelBounce(0);
    StepPanelBounce(1);
}
asm(".global sub_0804B3CC\n.thumb_set sub_0804B3CC, StepPanelBounceBothSides\n");
