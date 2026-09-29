#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080392BC.
 * sub_080392BC @ 0x080392BC
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
 * One pointer passed through. CoPowerPortrait_Draw reads +0x2c and +0x30 off r0, and
 * its two siblings CoPowerPortrait_SlideInLoop / CoPowerPortrait_SlideOutLoop hand the identical pointer
 * to Proc_Break -- which is what makes it a proc rather than a bare
 * pointer.
 */
void CoPowerPortrait_Draw2(ProcPtr proc)
{
    CoPowerPortrait_Draw(proc);
}
asm(".global sub_080392BC\n.thumb_set sub_080392BC, CoPowerPortrait_Draw2\n");
