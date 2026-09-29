#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DB50.
 * sub_0805DB50 @ 0x0805DB50, sub_0805DB64 @ 0x0805DB64
 */

/* F018: `push {lr}; bl a; bl b; bl c; pop {r0}; bx r0`.
 * THREE INDEPENDENT STATEMENTS, not a nest. Nothing moves r0 between the `bl`s,
 * which by itself does not separate `c(b(a()))` from three calls -- both leave
 * r0 alone. What settles it is the callees: all three take no arguments (none
 * of them reads r0-r3 before writing it) and all three end `pop {r0}`, i.e.
 * void, so there is no value to nest. src/decomp/c_08048558.c is the matched
 * exemplar of the same shape. */
void AiDeliberateDirectAttackOrRoleMove(void)
{
    AiStayHomeIfEnemyInfantryNear();
    AiTryAttack();
    AiRunRoleMove();
}
asm(".global sub_0805DB50\n.thumb_set sub_0805DB50, AiDeliberateDirectAttackOrRoleMove\n");

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
void AiDeliberateClearEscortTally(void)
{
    AiClearEscortTally();
}
asm(".global sub_0805DB64\n.thumb_set sub_0805DB64, AiDeliberateClearEscortTally\n");
