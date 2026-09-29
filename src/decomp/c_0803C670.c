#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803C670.
 * sub_0803C670 @ 0x0803C670, sub_0803C750 @ 0x0803C750, sub_0803C784 @ 0x0803C784, sub_0803C814 @ 0x0803C814
 */

/* Seven counted clears followed by one list walk. Every counter is a u8, which
 * is what puts the `lsl #0x18; lsr #0x18` on each increment; the bounds read
 * out of `cmp #K; bls` as `< K + 1` because gcc normalises an unsigned
 * `< const` into `<= const - 1`.
 *
 * The final loop is the same shape as GrantAllShopItems with the callback's second
 * argument 0 instead of 1 and no gate: rotated to a guard plus a do/while, and
 * fully strength-reduced except for the `i * 0x18` recomputation, which is what
 * a u8 counter that must be re-narrowed each iteration forces. */

void ResetCampaignUnlocks(void)
{
    u8 i;

    for (i = 0; i < 9; i++)
        SetMapCategoryUnlocked(i, 1);
    for (i = 0; i < 0xc0; i++)
        SetCampaignMapUnlocked(i, 1);
    for (i = 0; i < 0x13; i++)
        SetCoUnlocked(i, 1);
    for (i = 0; i < 0x13; i++)
        SetCoSelectable(i, 1);
    for (i = 0x60; i < 0xa0; i++)
        SetCampaignCompletionFlag(i, 0);
    for (i = 0x20; i < 0x60; i++)
        SetCampaignCompletionFlag(i, 0);
    for (i = 0; i < 0xc0; i++)
        SetMapPlayed(i, 0);
    for (i = 0; gUnknown_0849EDB0[i].unk04 != -1; i++)
        gUnknown_0849EDB0[i].unk0c(gUnknown_0849EDB0[i].unk10, 0);
}
asm(".global sub_0803C670\n.thumb_set sub_0803C670, ResetCampaignUnlocks\n");

/* Register a callback in the 16-slot gUnknown_02027FB0 table: claim the first
 * free slot, or bump the use count of the slot that already holds it, and
 * refuse once that count is above 1.
 *
 * The inner test must be spelled `<= 1` with the SUCCESS path as the `if` body
 * and `return 0` after it: the ROM's `cmp r0,#1; bhi <ret 0>` reaches the 0
 * from the conditional branch, so 0 is the FAR arm. Writing it the intuitive
 * way (`if (unk04 > 1) return 0;` then fall through) makes gcc emit `bls` plus
 * an extra `b` AND swaps the two arms of the outer test, because cross-jumping
 * then relocates the wrong block. Same instructions, mirrored layout.
 *
 * The tail `unk04++; return 1;` is written out in BOTH arms; cross-jumping is
 * what merges them, and the duplicated `ldrb r0, [r2, #4]` in each arm is the
 * proof it was two statements and not one shared one. Writing the tail once
 * after an `else if` chain needs a `continue` for the no-match case, and that
 * makes the loop optimiser replace the counter with a pointer end-compare
 * (`cmp r2, r3` against base + 0x78) -- the ROM keeps `i` in r3 against 0xf. */

bool8 TryClaimShopAvailSlot(int (*f)(int))
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if (gUnknown_02027FB0[i].unk00 == 0)
        {
            gUnknown_02027FB0[i].unk00 = f;
            gUnknown_02027FB0[i].unk04++;
            return 1;
        }
        if (gUnknown_02027FB0[i].unk00 == f)
        {
            if (gUnknown_02027FB0[i].unk04 <= 1)
            {
                gUnknown_02027FB0[i].unk04++;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}
asm(".global sub_0803C750\n.thumb_set sub_0803C750, TryClaimShopAvailSlot\n");

/* Rebuild the 16-slot gUnknown_02027FB0 registry, then fill `out` with the
 * indices of the gUnknown_0849EDB0 rows whose unk08 callback answers 1 AND
 * which TryClaimShopAvailSlot admits, terminated by 0xff and capped at 32 entries.
 *
 * The cap lives in the loop CONDITION, ahead of the list-end test: the ROM
 * checks `cmp r7, #0x1f; bgt` after the increments and before reloading unk04,
 * and the entry guard tests only unk04 because `n` is 0 there and folds away.
 *
 * `n = 0;` is its own statement before the `for`, not part of the init list --
 * the ROM sets r7 before r5.
 *
 * The two reads of `unk08` are what prove gUnknown_0849EDB0 is not `const`;
 * see the note on its declaration. */

void BuildShopItemList(u8 *out)
{
    int i;
    int n;

    for (i = 0; i < 16; i++)
    {
        gUnknown_02027FB0[i].unk00 = 0;
        gUnknown_02027FB0[i].unk04 = 0;
    }

    n = 0;
    for (i = 0; n < 32 && gUnknown_0849EDB0[i].unk04 != -1; i++)
    {
        if (gUnknown_0849EDB0[i].unk08(gUnknown_0849EDB0[i].unk10) == 1
            && TryClaimShopAvailSlot(gUnknown_0849EDB0[i].unk08))
        {
            out[n] = i;
            n++;
        }
    }
    out[n] = 0xff;
}
asm(".global sub_0803C784\n.thumb_set sub_0803C784, BuildShopItemList\n");

/* Walk the gUnknown_0849EDB0 list until a row's unk08 callback answers
 * something other than -1. The loop is fully strength-reduced: `i` survives
 * only as the byte offset in r4, and the exit test reuses the -1 that the
 * body's compare left in r1 -- the redundant-looking `adds r1, r0, #0` at the
 * call's return is what keeps it there. */

bool8 AnyShopItemRemaining(void)
{
    int i;

    for (i = 0; gUnknown_0849EDB0[i].unk04 != -1; i++)
    {
        if (gUnknown_0849EDB0[i].unk08(gUnknown_0849EDB0[i].unk10) != -1)
            return 1;
    }
    return 0;
}
asm(".global sub_0803C814\n.thumb_set sub_0803C814, AnyShopItemRemaining\n");
