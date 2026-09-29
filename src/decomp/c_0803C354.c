#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C354.
 * sub_0803C354 @ 0x0803C354, sub_0803C37C @ 0x0803C37C, sub_0803C3A4 @ 0x0803C3A4, sub_0803C3CC @ 0x0803C3CC, ShopAvail_MapNotOwned @ 0x0803C3F4, sub_0803C40C @ 0x0803C40C, sub_0803C434 @ 0x0803C434, sub_0803C45C @ 0x0803C45C, sub_0803C474 @ 0x0803C474, sub_0803C48C @ 0x0803C48C, ShopAvail_CoNeedsFlag24 @ 0x0803C4B4, ShopAvail_CoNeedsFlag25 @ 0x0803C4DC, ShopAvail_CoNeedsFlag26 @ 0x0803C504, ShopAvail_CoNeedsRank @ 0x0803C52C, ShopAvail_CoNeedsRank3 @ 0x0803C574, ShopAvail_CoNeedsRank4 @ 0x0803C580, ShopAvail_CoNeedsRank5 @ 0x0803C58C, ShopAvail_CoNeedsFlag21 @ 0x0803C598, ShopAvail_CoNeedsFlag22 @ 0x0803C5C0, sub_0803C5E8 @ 0x0803C5E8, sub_0803C614 @ 0x0803C614, sub_0803C620 @ 0x0803C620, sub_0803C62C @ 0x0803C62C, sub_0803C638 @ 0x0803C638, sub_0803C644 @ 0x0803C644
 */

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C354(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    if (!sub_0803CA9C(5))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C37C(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    if (!sub_0803CA9C(7))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C3A4(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    if (!sub_0803CA9C(2))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C3CC(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    if (!sub_0803CA9C(3))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int ShopAvail_MapNotOwned(u32 id)
{
    if (IsCampaignMapUnlockedByMapData(id))
        return -1;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
asm(".global sub_0803C3F4\n.thumb_set sub_0803C3F4, ShopAvail_MapNotOwned\n");
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C40C(void)
{
    if (IsCampaignCompletionFlagSet(0x20))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x21))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C434(void)
{
    if (IsCampaignCompletionFlagSet(0x28))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x21))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C45C(u32 id)
{
    if (sub_0803CA9C(id))
        return -1;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C474(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int sub_0803C48C(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x23))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int ShopAvail_CoNeedsFlag24(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x24))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
asm(".global sub_0803C4B4\n.thumb_set sub_0803C4B4, ShopAvail_CoNeedsFlag24\n");
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int ShopAvail_CoNeedsFlag25(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x25))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
asm(".global sub_0803C4DC\n.thumb_set sub_0803C4DC, ShopAvail_CoNeedsFlag25\n");
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int ShopAvail_CoNeedsFlag26(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x26))
        return 0;
    return 1;
}

/* The shared body behind ShopAvail_CoNeedsRank3 / ShopAvail_CoNeedsRank4 / ShopAvail_CoNeedsRank5, which
 * call it with n = 3, 4 and 5. Reads as "is this thing available at rank n":
 * -1 while gUnknown_02028030.unk2a already has the bit set, 0 when unlock 0x21
 * is not held or the rank is below n, and 1 otherwise.
 *
 * `n` is signed: the compare against GetCampaignScoreRank's result is `blt`.
 * IsCampaignCompletionFlagSet's result is tested with a bare `cmp r0, #0` while
 * IsCoUnlocked's needs `lsls r0, r0, #0x18` -- the two return widths side by
 * side in one function, which is the control the note on those prototypes in
 * unknown-functions.h cites.
 *
 * GetCampaignScoreRank is called TWICE on the same unk10, with the store to
 * gUnknown_030040D4 in between; the second `ldrh` is a genuine reload (the
 * store can alias), so this is two source expressions and not one CSE'd value.
 */

int ShopAvail_CoNeedsRank(u32 id, int n)
{
    if ((u8)IsCoUnlocked(id))
        return -1;

    if (IsCampaignCompletionFlagSet(0x21))
    {
        gUnknown_030040D4 = GetCampaignScoreRank(gUnknown_0200C420.unk10);

        if (GetCampaignScoreRank(gUnknown_0200C420.unk10) >= n)
            return 1;
    }

    return 0;
}

/* `return ShopAvail_CoNeedsRank(id, 3);` -- the incoming r0 passes straight through as
 * the first argument and the constant goes in r1. The epilogue is
 * `pop {r1}; bx r1`, NOT `pop {r0}; bx r0`, so the callee's result is live and
 * the `return` is load-bearing; and there is no `lsls #24; lsrs #24` after the
 * `bl`, which fixes ShopAvail_CoNeedsRank's return at int width. */

int ShopAvail_CoNeedsRank3(u32 id)
{
    return ShopAvail_CoNeedsRank(id, 3);
}

/* `return ShopAvail_CoNeedsRank(id, 4);` -- the incoming r0 passes straight through as
 * the first argument and the constant goes in r1. The epilogue is
 * `pop {r1}; bx r1`, NOT `pop {r0}; bx r0`, so the callee's result is live and
 * the `return` is load-bearing; and there is no `lsls #24; lsrs #24` after the
 * `bl`, which fixes ShopAvail_CoNeedsRank's return at int width. */

int ShopAvail_CoNeedsRank4(u32 id)
{
    return ShopAvail_CoNeedsRank(id, 4);
}

/* `return ShopAvail_CoNeedsRank(id, 5);` -- the incoming r0 passes straight through as
 * the first argument and the constant goes in r1. The epilogue is
 * `pop {r1}; bx r1`, NOT `pop {r0}; bx r0`, so the callee's result is live and
 * the `return` is load-bearing; and there is no `lsls #24; lsrs #24` after the
 * `bl`, which fixes ShopAvail_CoNeedsRank's return at int width. */

int ShopAvail_CoNeedsRank5(u32 id)
{
    return ShopAvail_CoNeedsRank(id, 5);
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
asm(".global sub_0803C504\n.thumb_set sub_0803C504, ShopAvail_CoNeedsFlag26\n");
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int ShopAvail_CoNeedsFlag21(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x21))
        return 0;
    return 1;
}

/* One of the 0x0803C354-0x0803C670 tri-state predicates: -1 (blocked),
asm(".global sub_0803C598\n.thumb_set sub_0803C598, ShopAvail_CoNeedsFlag21\n");
asm(".global sub_0803C58C\n.thumb_set sub_0803C58C, ShopAvail_CoNeedsRank5\n");
asm(".global sub_0803C580\n.thumb_set sub_0803C580, ShopAvail_CoNeedsRank4\n");
asm(".global sub_0803C574\n.thumb_set sub_0803C574, ShopAvail_CoNeedsRank3\n");
asm(".global sub_0803C52C\n.thumb_set sub_0803C52C, ShopAvail_CoNeedsRank\n");
 * 1 (satisfied) or 0 (not yet). See the comment on sub_0803CA9C in
 * include/unknown-functions.h for why the bit readers return `u8`.
 */

int ShopAvail_CoNeedsFlag22(u32 id)
{
    if ((u8)IsCoUnlocked(id))
        return -1;
    if (!IsCampaignCompletionFlagSet(0x22))
        return 0;
    return 1;
}

/* The shared body behind the five byte-identical wrappers at
 * 0x0803C614/620/62C/638/644.
 *
 * Two things are read straight off the assembly and neither is optional.
 * The `lsls r0, r0, #0x18` after each `bl` is the eight-bit return type of the
 * gUnknown_02028030 bit readers (see the comment on IsCoUnlocked in
 * unknown-functions.h); an `int` return emits no shift.
 * And the SECOND test's polarity is inverted from the natural spelling: the
 * ROM's `beq` jumps to the `movs r0, #0` block and falls through into
 * `movs r0, #1`, so per the branch-polarity rule the first `return` in the
 * source is the ZERO. Writing the obvious `if (IsCoUnlocked(id)) return 1;
 * return 0;` swaps those two blocks and misses at the same length.
 */

int sub_0803C5E8(u32 id)
{
    if (sub_0803CAD4(id))
        return -1;

    if (!(u8)IsCoUnlocked(id))
        return 0;

    return 1;
}

/* `return sub_0803C5E8(id);` -- one of five byte-identical forwarders at
 * 0x0803C614/620/62C/638/644. Nothing sets up an argument register, so the
 * incoming r0 is passed through unchanged, and `pop {r1}; bx r1` says the
 * result is returned rather than discarded. */

int sub_0803C614(u32 id)
{
    return sub_0803C5E8(id);
}

/* `return sub_0803C5E8(id);` -- one of five byte-identical forwarders at
 * 0x0803C614/620/62C/638/644. Nothing sets up an argument register, so the
 * incoming r0 is passed through unchanged, and `pop {r1}; bx r1` says the
 * result is returned rather than discarded. */

int sub_0803C620(u32 id)
{
    return sub_0803C5E8(id);
}

/* `return sub_0803C5E8(id);` -- one of five byte-identical forwarders at
 * 0x0803C614/620/62C/638/644. Nothing sets up an argument register, so the
 * incoming r0 is passed through unchanged, and `pop {r1}; bx r1` says the
 * result is returned rather than discarded. */

int sub_0803C62C(u32 id)
{
    return sub_0803C5E8(id);
}

/* `return sub_0803C5E8(id);` -- one of five byte-identical forwarders at
 * 0x0803C614/620/62C/638/644. Nothing sets up an argument register, so the
 * incoming r0 is passed through unchanged, and `pop {r1}; bx r1` says the
 * result is returned rather than discarded. */

int sub_0803C638(u32 id)
{
    return sub_0803C5E8(id);
}

/* `return sub_0803C5E8(id);` -- one of five byte-identical forwarders at
 * 0x0803C614/620/62C/638/644. Nothing sets up an argument register, so the
 * incoming r0 is passed through unchanged, and `pop {r1}; bx r1` says the
 * result is returned rather than discarded. */

int sub_0803C644(u32 id)
{
    return sub_0803C5E8(id);
}

asm(".global sub_0803C5C0\n.thumb_set sub_0803C5C0, ShopAvail_CoNeedsFlag22\n");
