#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DDF4.
 * sub_0803DDF4 @ 0x0803DDF4, sub_0803DE08 @ 0x0803DE08
 */

/* F002: `push {lr}; ldr r0,=g; movs r1,#0; bl sub_080152EC; pop {r0}; bx r0`.
 * One statement, result discarded -- `pop {r0}` makes the wrapper void.
 * sub_080152EC's first parameter is `const void *`, which takes the
 * `const u8 []` blob with no cast; a plain `void *` would discard the const and
 * -Werror turns that into an error. src/decomp/c_0802BF80.c is the matched
 * exemplar for this shape. */
void StartDebugArmyEditor(void)
{
    sub_080152EC(gUnknown_0849F628, 0);
}
asm(".global sub_0803DDF4\n.thumb_set sub_0803DDF4, StartDebugArmyEditor\n");

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
void sub_0803DE08(void)
{
    GrantAllShopItems();
}
