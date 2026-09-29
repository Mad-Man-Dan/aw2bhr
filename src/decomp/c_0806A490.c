#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A490.
 * sub_0806A490 @ 0x0806A490, sub_0806A4A0 @ 0x0806A4A0
 */

#include "proc.h"

/* One of family F000's 16-byte forwarders: `push {lr}; ldr r0,=script;
 * bl Proc_EndEach; pop {r0}; bx r0`. `pop {r0}` fixes it as void, so the call
 * is a bare statement and nothing is forwarded. The script is ROM data reached
 * only as an address; see the evidence for gUnknown_08580FF4 in
 * include/unknown-globals.h.
 */

void EndIntroBgZoom(void)
{
    Proc_EndEach(gUnknown_08580FF4);
}
asm(".global sub_0806A490\n.thumb_set sub_0806A490, EndIntroBgZoom\n");

/* One of family F000's 16-byte forwarders: `push {lr}; ldr r0,=script;
 * bl Proc_EndEach; pop {r0}; bx r0`. `pop {r0}` fixes it as void, so the call
 * is a bare statement and nothing is forwarded. The script is ROM data reached
 * only as an address; see the evidence for gUnknown_08581108 in
 * include/unknown-globals.h.
 */

void EndIntroCoSlides(void)
{
    Proc_EndEach(gUnknown_08581108);
}
asm(".global sub_0806A4A0\n.thumb_set sub_0806A4A0, EndIntroCoSlides\n");
