#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045FBC.
 * sub_08045FBC @ 0x08045FBC
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
 * Byte-identical twin of MapEventFx_EndScreenShake, 0xe8 bytes below it: same callee,
 * same shape. Two distinct source functions is all the ROM proves.
 */
void MapEventFx_EndScreenShake2(void)
{
    EndScreenShake();
}
asm(".global sub_08045FBC\n.thumb_set sub_08045FBC, MapEventFx_EndScreenShake2\n");
