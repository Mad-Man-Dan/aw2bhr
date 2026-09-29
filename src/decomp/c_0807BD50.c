#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BD50.
 * sub_0807BD50 @ 0x0807BD50
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
 * One pointer passed through. MissionTitleName_UpdateSpin opens `ldr r2,[r0,#0x58]` and
 * writes the decremented value back, so r0 is read before it is written;
 * +0x58 is past PROC_HEADER.
 */
void MissionTitleName_UpdateSpin2(ProcPtr proc)
{
    MissionTitleName_UpdateSpin(proc);
}
asm(".global sub_0807BD50\n.thumb_set sub_0807BD50, MissionTitleName_UpdateSpin2\n");
