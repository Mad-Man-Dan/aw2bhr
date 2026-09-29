#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080767C0.
 * sub_080767C0 @ 0x080767C0
 */

#include "proc.h"

/* Two calls to StartHeaderBanner differing only in the blob, exactly the shape
 * c_0806EB5C.c uses. Both arms pass a literal 0 as the sixth argument; the ROM
 * spells the second one `str r4, [sp, #4]` because r4 holds IsHardCampaignMode's
 * result and the `beq` has just proved it zero -- cse's record_jump_equiv, the
 * same fold c_08075368.c records on its comparison zero, and not a sign that
 * the two arms pass different things. */
void StartWorldMapTitleBanner(ProcPtr proc)
{
    if (IsHardCampaignMode())
        StartHeaderBanner(gUnknown_08614538, gUnknown_02010C50, 0x200, 0xE, 0x10, 0,
                     (int)proc);
    else
        StartHeaderBanner(gUnknown_0861452C, gUnknown_02010C50, 0x200, 0xE, 0x10, 0,
                     (int)proc);
}
asm(".global sub_080767C0\n.thumb_set sub_080767C0, StartWorldMapTitleBanner\n");
