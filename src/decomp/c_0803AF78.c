#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AF78.
 * sub_0803AF78 @ 0x0803AF78, sub_0803AF84 @ 0x0803AF84, sub_0803AF90 @ 0x0803AF90
 */

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * The callee is EMPTY -- a lone `bx lr`, four bytes. void(void) and not
 * void(int): at the only call site (DebugBackupUtility_Loop) r0 holds the `1 & flags`
 * of the test that just branched, which is a leftover and not an argument
 * anyone set up.
 */
void sub_0803AF78(void)
{
    sub_0801B4B8();
}

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * Same as sub_0803AF78 twelve bytes above: the callee sub_0801B4BC is a
 * lone `bx lr`, and DebugBackupUtility_Loop leaves `4 & flags` in r0 at the call.
 */
void sub_0803AF84(void)
{
    sub_0801B4BC();
}

/* Family F005 (data/families.json): 19 sixteen-byte wrappers shaped
 * `push {lr}; bl S; bl S; pop {r0}; bx r0` -- two calls, nothing between
 * them, `pop {r0}` so void.
 * Two statements and not `g(f())`: the second callee takes no arguments (it
 * opens with a `bl`, a pool `ldr` or a `movs` into r0, never a read of r0),
 * so there is no parameter for the first call's result to reach. `g(f())`
 * would be byte-identical only if `g` had a parameter, and none of the 19
 * second callees does -- see the F005 block in unknown-functions.h. */

void sub_0803AF90(void)
{
    FormatSaveSectors();
    LoadProfile();
}
