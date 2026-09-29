#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BB88.
 * sub_0801BB88 @ 0x0801BB88
 */

/* Splits the pending OAM copy in two at object `a`: gOamTransferHead takes
 * the first `a` objects and gOamTransferTail the remaining 0x80 - a.
 * 0x07000000 is OAM and each object is 8 bytes, hence the shared `a * 8`
 * byte offset.
 *
 * The `strh r0, [r2, #0xa]` with no narrowing is what fixes the parameter at
 * 16 bits or wider; `int` and `u16` are byte-identical here.
 */
void InitOam(int a)
{
    gOamTransferHead.src = gUnknown_03002520;
    gOamTransferHead.dst = (void *)0x07000000;
    gOamTransferHead.oamOffset = 0;
    gOamTransferHead.objectCount = a;
    gOamTransferTail.src = (u8 *)gUnknown_03002520 + a * 8;
    gOamTransferTail.dst = (u8 *)0x07000000 + a * 8;
    gOamTransferTail.oamOffset = a * 8;
    gOamTransferTail.objectCount = 0x80 - a;
}
asm(".global sub_0801BB88\n.thumb_set sub_0801BB88, InitOam\n");
