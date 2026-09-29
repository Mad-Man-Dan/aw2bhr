#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CA70.
 * sub_0803CA70 @ 0x0803CA70
 */

/* `id = FindMapIdByMapData(id)` -- the call result is assigned back over the
 * PARAMETER, and that is what produces the otherwise inexplicable
 * `adds r3, r0, #0` before the `bl`. r3 is call-clobbered, so the copy is dead
 * on any reading; agbcc emits it because the parameter's home pseudo is
 * written before being overwritten by the return value. Spelling the result
 * into a fresh local instead drops that instruction and the function comes out
 * 4 bytes short (measured side by side).
 *
 * Everything after the call is the promoted IsCampaignMapUnlocked verbatim, with the
 * same `s = &gUnknown_02028030` two-step that keeps `adds r0, #0x12` out of
 * the relocation addend -- and the assignment must sit AFTER the call or the
 * address is held across it in a callee-saved register. */
u8 IsCampaignMapUnlockedByMapData(u32 id)
{
    struct Unk02028030 *s;
    u32 idx;
    u8 *b;
    u8 *p;

    id = FindMapIdByMapData(id);
    s = &gUnknown_02028030;
    idx = id >> 3;
    b = s->unk12;
    p = b + idx;

    return (1 << (id & 7)) & *p;
}
asm(".global sub_0803CA70\n.thumb_set sub_0803CA70, IsCampaignMapUnlockedByMapData\n");
