#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C5AC.
 * sub_0802C5AC @ 0x0802C5AC
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
 * CloseTopMenu RETURNS a value -- it ends `pop {r1}; bx r1` and is
 * declared s8 -- and this forwarder still pops into r0, so the result
 * is discarded and the forwarder is void. sub_08019E38, CloseTopMenu3
 * and CloseTopMenu2 are the three byte-identical copies of it.
 */
void CloseTopMenu2(void)
{
    CloseTopMenu();
}
asm(".global sub_0802C5AC\n.thumb_set sub_0802C5AC, CloseTopMenu2\n");
