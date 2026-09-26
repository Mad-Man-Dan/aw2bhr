#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041DC0.
 * sub_08041DC0 @ 0x08041DC0, sub_08041DCC @ 0x08041DCC, sub_08041DD8 @ 0x08041DD8, sub_08041DE8 @ 0x08041DE8
 */

/* Family F003 (data/families.json): 34 twelve-byte forwarders shaped
 * `push {lr}; movs r0,#K; bl S; pop {r0}; bx r0`, i.e. one call with one
 * literal argument and nothing else. `pop {r0}; bx r0` fixes this as void.
 * The argument's C type is not recoverable from here -- `movs r0,#K` is the
 * same two bytes for int/u32/u16/s16/u8 -- so the spelling follows the
 * callee's declaration in unknown-functions.h and nothing else.
 * Exemplar: src/decomp/c_08004A60.c. */

void sub_08041DC0(void)
{
    sub_0803B6E8(0xC0);
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
 * The callee reads no argument register before writing it, so there
 * is no parameter to pass through either.
 */
void sub_08041DCC(void)
{
    sub_0803B774();
}

/* Family F005 (data/families.json): 19 sixteen-byte wrappers shaped
 * `push {lr}; bl S; bl S; pop {r0}; bx r0` -- two calls, nothing between
 * them, `pop {r0}` so void.
 * Two statements and not `g(f())`: the second callee takes no arguments (it
 * opens with a `bl`, a pool `ldr` or a `movs` into r0, never a read of r0),
 * so there is no parameter for the first call's result to reach. `g(f())`
 * would be byte-identical only if `g` had a parameter, and none of the 19
 * second callees does -- see the F005 block in unknown-functions.h. */

void sub_08041DD8(void)
{
    sub_08023348();
    sub_0803662C();
}

/* Family F005 (data/families.json): 19 sixteen-byte wrappers shaped
 * `push {lr}; bl S; bl S; pop {r0}; bx r0` -- two calls, nothing between
 * them, `pop {r0}` so void.
 * Two statements and not `g(f())`: the second callee takes no arguments (it
 * opens with a `bl`, a pool `ldr` or a `movs` into r0, never a read of r0),
 * so there is no parameter for the first call's result to reach. `g(f())`
 * would be byte-identical only if `g` had a parameter, and none of the 19
 * second callees does -- see the F005 block in unknown-functions.h. */

void sub_08041DE8(void)
{
    sub_080258CC();
    LockUnitSelection();
}
