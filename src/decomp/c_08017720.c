#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017720.
 * sub_08017720 @ 0x08017720
 */



/*
 * InsertBestScoreRecord -- insert an entry into a unit's ranked list.
 *
 * Does nothing unless player 1's team is alive. Which list depends on the game
 * mode: mode 2 uses the five-slot rows of gUnknown_0200C078 with `b` biased by
 * 0x6c, mode 1 the two-slot rows of gUnknown_0200C2D0 with `b` biased by 0x8a.
 * Any other mode is ignored. Each slot is one word holding three bitfields:
 * .unk00_00 at bit 0, .unk00_08 at bit 8 and .unk00_14 at bit 20.
 *
 * Mode 2 keeps the row sorted on .unk00_14 descending, and on .unk00_08
 * ascending where two entries tie on .unk00_14. The search walks the five slots
 * for the first one the new entry outranks, the entries from there down shift
 * one slot along (the last is pushed off the end), and `a`, `c` and `d` go into
 * the hole. A row whose five entries all outrank the new one is left alone.
 * Mode 1 does not search: the slot is picked by IsHardCampaignMode(), and is
 * only overwritten when its .unk00_14 does not already beat `c`.
 *
 * The comparisons are signed with no cast in sight, because a 12-bit unsigned
 * bitfield promotes to int.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The search is a `goto` loop with explicit labels. A `while` or `for`
 *     merges the entry test with the back-edge test, where the original tests
 *     the same thing in both places.
 *   - .unk00_14 is read through a `volatile u16 *` overlay. Read as a plain
 *     bitfield, the compiler shares one word load with .unk00_08 and reads it
 *     once where the original reads it three times per pass. Making the whole
 *     struct volatile instead selects word loads and is also wrong.
 *   - The search walks the pointer `e` while the shift and the stores index
 *     gUnknown_0200C078 directly. The original reaches the array both ways in
 *     this one function.
 *   - IsHardCampaignMode() is written out at each of the four uses. It is a
 *     call, so the compiler cannot fold the repeats away.
 *   - `case 2` comes before `case 1`: the compiler emits the bodies in source
 *     order while testing the values in ascending order, which is what the
 *     original does.
 */
void InsertBestScoreRecord(int a, int b, int c, int d)
{
    int k;
    int j;
    struct Unk0200C078Rec *e;

    if (IsPlayer1TeamAlive() == 0)
        return;

    switch (gPlaySt.gameMode)
    {
    case 2:
        b -= 0x6c;
        k = 0;
        e = &gUnknown_0200C078[b].unk00[0];
        if ((*(volatile u16 *)((u8 *)e + 2) >> 4) < c)
            goto search_done;
search_loop:
        if ((*(volatile u16 *)((u8 *)e + 2) >> 4) == c
            && e->unk00_08 > d)
            goto search_done;
        e++;
        k++;
        if (k > 4)
            goto search_done;
        if ((*(volatile u16 *)((u8 *)e + 2) >> 4) >= c)
            goto search_loop;
search_done:
        if (k == 5)
            return;
        for (j = 3; j >= k; j--)
            gUnknown_0200C078[b].unk00[j + 1] = gUnknown_0200C078[b].unk00[j];
        gUnknown_0200C078[b].unk00[k].unk00_00 = a;
        gUnknown_0200C078[b].unk00[k].unk00_14 = c;
        gUnknown_0200C078[b].unk00[k].unk00_08 = d;
        break;

    case 1:
        b -= 0x8a;
        if (gUnknown_0200C2D0[b].unk00[IsHardCampaignMode()].unk00_14 > c)
            return;
        gUnknown_0200C2D0[b].unk00[IsHardCampaignMode()].unk00_00 = a;
        gUnknown_0200C2D0[b].unk00[IsHardCampaignMode()].unk00_14 = c;
        gUnknown_0200C2D0[b].unk00[IsHardCampaignMode()].unk00_08 = d;
        break;
    }
}
asm(".global sub_08017720\n.thumb_set sub_08017720, InsertBestScoreRecord\n");
