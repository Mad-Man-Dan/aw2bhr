#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072598.
 * sub_08072598 @ 0x08072598
 */

#include "proc.h"

/* One of family F000's 16-byte forwarders: `push {lr}; ldr r0,=script;
 * bl Proc_EndEach; pop {r0}; bx r0`. `pop {r0}` fixes it as void, so the call
 * is a bare statement and nothing is forwarded. The script is ROM data reached
 * only as an address; see the evidence for gUnknown_08613F2C in
 * include/unknown-globals.h.
 */

void DeleteAllPaletteAnimator(void)
{
    Proc_EndEach(gUnknown_08613F2C);
}
asm(".global sub_08072598\n.thumb_set sub_08072598, DeleteAllPaletteAnimator\n");
