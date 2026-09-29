#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085298.
 * sub_08085298 @ 0x08085298
 */

/* Family F005 (data/families.json): 19 sixteen-byte wrappers shaped
 * `push {lr}; bl S; bl S; pop {r0}; bx r0` -- two calls, nothing between
 * them, `pop {r0}` so void.
 * Two statements and not `g(f())`: the second callee takes no arguments (it
 * opens with a `bl`, a pool `ldr` or a `movs` into r0, never a read of r0),
 * so there is no parameter for the first call's result to reach. `g(f())`
 * would be byte-identical only if `g` had a parameter, and none of the 19
 * second callees does -- see the F005 block in unknown-functions.h.
 *
 * RETYPED IN WAVE 20 (W20-C): ONE parameter, unused. The F005 block declared
 * every member `void f(void)`, but the `pop {r0}` rule it cites settles the
 * RETURN type only -- the empty parameter list was never argued. All four
 * callers set r0 from a callee-saved proc pointer in the instruction
 * immediately before the `bl` (`adds r0, r4, #0` / `adds r0, r7, #0` at
 * 0x08084xxx), which is argument setup and nothing else. The parameter is dead
 * here -- the first `bl` overwrites r0 -- so this definition is byte-identical
 * before and after (re-verified with trymatch). Same correction as
 * DisableWindow0AndResetMapLayers, and those two are the only F005 members whose callers set any
 * argument register; the other seventeen really are nullary. */

void CoInfoScreen_ClearBg0(ProcPtr proc)
{
    ClearBg0Tilemap();
    BG_EnableSyncBG0();
}
asm(".global sub_08085298\n.thumb_set sub_08085298, CoInfoScreen_ClearBg0\n");
