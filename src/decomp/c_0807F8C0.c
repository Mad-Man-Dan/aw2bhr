#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F8C0.
 * sub_0807F8C0 @ 0x0807F8C0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* One of family F000's 16-byte forwarders: `push {lr}; ldr r0,=script;
 * bl Proc_EndEach; pop {r0}; bx r0`. `pop {r0}` fixes it as void, so the call
 * is a bare statement and nothing is forwarded. The script is ROM data reached
 * only as an address; see the evidence for ProcScr_CoSelect in
 * include/unknown-globals.h.
 */

void EndCoSelect_0807F8C1(void)
{
    Proc_EndEach(ProcScr_CoSelect);
}

asm(".global sub_0807F8C0\n.thumb_set sub_0807F8C0, EndCoSelect_0807F8C1\n");
