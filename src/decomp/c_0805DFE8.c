#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DFE8.
 * sub_0805DFE8 @ 0x0805DFE8
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
 * The callee reads no argument register before writing it, so there
 * is no parameter to pass through either.
 */
void AiDeliberateIndirectRoleMove(void)
{
    AiRunRoleMove();
}
asm(".global sub_0805DFE8\n.thumb_set sub_0805DFE8, AiDeliberateIndirectRoleMove\n");
