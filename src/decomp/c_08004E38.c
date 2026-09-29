#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004E38.
 * sub_08004E38 @ 0x08004E38
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
 * Two parameters, passed straight through: the forwarder sets up no
 * argument register, and sub_0808B678 (src/decomp/c_0808B678.c) is a strcpy
 * taking `char *, const char *`. Its caller DesignRoomLoadFromSlot supplies both
 * (`ldr r0,[r4]; adds r0,#0x9c; adds r1, r5, #0`). The char * result is
 * dropped -- `pop {r0}`, not `pop {r1}`.
 */
void sub_08004E38(char * dst, const char * src)
{
    sub_0808B678(dst, src);
}
