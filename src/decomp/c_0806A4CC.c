#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A4CC.
 * sub_0806A4CC @ 0x0806A4CC
 */

#include "proc.h"

/* One of family F000's 16-byte forwarders: `push {lr}; ldr r0,=script;
 * bl Proc_EndEach; pop {r0}; bx r0`. `pop {r0}` fixes it as void, so the call
 * is a bare statement and nothing is forwarded. The script is ROM data reached
 * only as an address; see the evidence for gUnknown_08581138 in
 * include/unknown-globals.h.
 */

void EndIntroBlendFade(void)
{
    Proc_EndEach(gUnknown_08581138);
}
asm(".global sub_0806A4CC\n.thumb_set sub_0806A4CC, EndIntroBlendFade\n");
