#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BBC4.
 * sub_0801BBC4 @ 0x0801BBC4, sub_0801BC08 @ 0x0801BC08
 */

/* Flushes the second pending-copy descriptor unconditionally, then latches
 * where it came from. The `* 2` is the halfword count turned into bytes for
 * CpuFastSet, exactly as the note on struct OamTransfer already records for
 * SyncLoOam.
 *
 * SyncLoOam forty bytes below is the same body over gOamTransferHead and
 * guarded on objectCount; SyncHiOamNoCopy is this body with the CpuFastSet
 * removed. All three keep gOamTransferTail's address in r4 across their calls,
 * which is what makes the descriptor a single object rather than five loose
 * globals. */
void SyncHiOam(void)
{
    CpuFastSet(gOamTransferTail.src, gOamTransferTail.dst,
               gOamTransferTail.objectCount * 2);
    sub_080718E8(gOamTransferTail.src, gOamTransferTail.objectCount);
    gUnknown_03002F2C = gOamTransferTail.src;
    gUnknown_030030D4 = gUnknown_03002520;
    gUnknown_030024C0 = 0;
}
asm(".global sub_0801BBC4\n.thumb_set sub_0801BBC4, SyncHiOam\n");

/* SyncHiOam's body over the OTHER descriptor, guarded on a non-zero count
 * and latching into a different global. The guard is the whole difference
 * between the two, and it is why this one is 52 bytes to its neighbour's 68
 * despite doing the same work. */
void SyncLoOam(void)
{
    if (gOamTransferHead.objectCount != 0)
    {
        CpuFastSet(gOamTransferHead.src, gOamTransferHead.dst,
                   gOamTransferHead.objectCount * 2);
        sub_080718E8(gOamTransferHead.src, gOamTransferHead.objectCount);
        gUnknown_0300141C = gOamTransferHead.src;
    }
}
asm(".global sub_0801BC08\n.thumb_set sub_0801BC08, SyncLoOam\n");
