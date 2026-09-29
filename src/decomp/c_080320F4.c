#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080320F4.
 * sub_080320F4 @ 0x080320F4
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Like EndLinkTransferPercent: gUnknown_0849B304 has no starter in the ROM, so its
 * ProcCmd typing rests on Proc_EndEach's prototype and on the blob being a
 * whole number of 8-byte commands (0xC8 = 25).
 */

void EndLinkLeaveProc(void)
{
    Proc_EndEach(gUnknown_0849B304);
}
asm(".global sub_080320F4\n.thumb_set sub_080320F4, EndLinkLeaveProc\n");
