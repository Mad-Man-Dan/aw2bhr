#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038368.
 * sub_08038368 @ 0x08038368, sub_0803840C @ 0x0803840C, sub_08038434 @ 0x08038434
 */

/* MATCHED byte-for-byte (wave 43, W43-E), first attempt.
 *
 * NEEDS ITS .rodata POOL WORD PLACED: "rodata": ["0x08090F08"]. That ROM word
 * holds 0x0200C420, i.e. &gUnknown_0200C420 -- an -fforce-addr address
 * constant, not a global. The plain `gUnknown_0200C420.unk38[i]` subscript
 * emits the two-level chain the ROM has, including the reload of the base
 * through the pool word on every iteration.
 *
 * NOTE FOR PROMOTION: 0x08090EFC (sub_080381C0's pool word, same block) and
 * 0x08090F08 are NOT adjacent -- 0x08090F00 and 0x08090F04 sit between them and
 * are EndOfGame_PrepareSummary's. AN EARLIER VERSION OF THIS NOTE WARNED THAT A UNIT
 * HOLDING BOTH WOULD BE REJECTED BY tools/split_rodata.py. THAT WAS WRONG and
 * W43-F disproved it; the corrected rule is in docs/agbcc-codegen.md. Ownership
 * across this run is monotonic in .text address with no unclaimed word --
 * EFC/F00,F04/F08/F0C/F10 belong to 381C0/38240/38368/389D8/38AD8 in that
 * order -- and a promotion unit is a CONTIGUOUS .text range, so any unit
 * spanning two of these functions necessarily contains every function between
 * them and therefore claims every word between their words. Concretely:
 * 0x080381C0 + 0x80 = 0x08038240, so EndOfGame_PrepareSummary immediately follows
 * sub_080381C0 and lies strictly inside any span reaching SaveCampaignMissionResult; a unit
 * with both my words but not F00/F04 cannot be drawn.
 *   What the check actually needs is that no word in the span is unclaimed or
 * claimed out of order -- NOT that one function's own words are adjacent.
 *
 * This function is what PROVED gUnknown_0200C420.unk38 is an array of the
 * 8/12/12 bitfield record `struct Unk0200C078Rec` rather than the lone `u32`
 * wave 37 carved: it writes all three fields at the three widths that layout
 * forces and nothing else reproduces -- `strb` for bits 0..7, `ldr; and
 * 0xFFF000FF; orr; str` for bits 8..19, and `ldrh [+0x3a]; and 0xf; orr; strh`
 * for bits 20..31.
 *
 * The `if (i > 0x28) return;` after the loop is real source and not a
 * duplicated exit test. The loop's own bound failure jumps straight to the
 * epilogue because agbcc knows i is 0x29 there, but the two `break` paths fall
 * into the test, which is why it is emitted a second time at _080383BA.
 */

void SaveCampaignMissionResult(int a1, int a2, int a3)
{
    u8 found;
    u8 i;

    found = 0;
    for (i = 0; i <= 0x28; i++)
    {
        if (gUnknown_0200C420.unk38[i].unk00_08 == 0)
            break;
        if (gUnknown_0200C420.unk38[i].unk00_00 == a1)
        {
            found = 1;
            break;
        }
    }
    if (i > 0x28)
        return;

    gUnknown_0200C420.unk38[i].unk00_00 = a1;
    gUnknown_0200C420.unk38[i].unk00_08 = a2;
    gUnknown_0200C420.unk38[i].unk00_14 = a3;

    if (found == 0)
        gUnknown_0200C420.unk38[i + 1].unk00_08 = 0;
}
asm(".global sub_08038368\n.thumb_set sub_08038368, SaveCampaignMissionResult\n");

/* MATCHED byte-for-byte (wave 43, W43-E), first attempt. relocs: match.
 *
 * Returns the 1-based length of the live run of gUnknown_0200C420.unk38[],
 * where "live" is the bits-8..19 field being non-zero. Reads as
 * `ldr [.,#0x38]; lsls #0xc; lsrs #0x14`, the unsigned extraction of a 12-bit
 * bitfield at bit 8 -- and NOT a mask plus a shift.
 *
 * The index is `u8`: it is truncated with an `lsls #0x18; lsrs #0x18` pair on
 * every iteration and the bound test is `bhi`, unsigned. It stays a real
 * counter rather than being strength-reduced into a pointer (which is what
 * happens to the identically-shaped guard in the adjacent GetAverageCampaignScore)
 * precisely BECAUSE the index value is needed after the loop for `i + 1`.
 * Those two functions are one shape differing in what they do with the walk,
 * and the u8-vs-int index difference falls straight out of that.
 *
 * The bound check sits after the increment and the first iteration jumps past
 * it into the field test: ordinary loop rotation, with `i = 0 <= 0x28` folded
 * away on entry. Bound failure leaves i at 0x29, so the tail returns 0x2a.
 */

int GetCampaignResultCountPlusOne(void)
{
    u8 i;

    for (i = 0; i <= 0x28; i++)
    {
        if (gUnknown_0200C420.unk38[i].unk00_08 == 0)
            break;
    }
    return i + 1;
}
asm(".global sub_0803840C\n.thumb_set sub_0803840C, GetCampaignResultCountPlusOne\n");

/* MATCHED byte-for-byte (wave 43, W43-E), first attempt. relocs: match.
 *
 * The mean of the bits-20..31 field over the live run of
 * gUnknown_0200C420.unk38[] -- Div(sum, count), with the same run guard
 * GetCampaignResultCountPlusOne walks (bits 8..19 non-zero, index bounded at 0x28 inclusive).
 * Diffing the two streams against each other before drafting is what made this
 * one free: it is GetCampaignResultCountPlusOne's loop with an accumulator instead of a
 * returned index.
 *
 * The index is `int` here, not the `u8` GetCampaignResultCountPlusOne uses, and the difference
 * is not a choice: nothing reads the index after the loop, so strength_reduce
 * replaces it with a pointer walking the struct base (`adds r2,#4`) and the
 * bound becomes the address compare `cmp r2,r4; bgt` against a preheader
 * `base + 0xa0`. That preheader add is the biv init and must NOT be authored.
 *
 * `ldrh [.,#0x3a]; lsrs #4` with no mask is the unsigned read of the 12-bit
 * field at bits 20..31: it ends at the top of the upper halfword, so the
 * logical shift alone extracts it.
 */

int GetAverageCampaignScore(void)
{
    int sum;
    int count;
    int i;

    sum = 0;
    count = 0;
    for (i = 0; i <= 0x28; i++)
    {
        if (gUnknown_0200C420.unk38[i].unk00_08 == 0)
            break;
        count++;
        sum += gUnknown_0200C420.unk38[i].unk00_14;
    }
    return Div(sum, count);
}
asm(".global sub_08038434\n.thumb_set sub_08038434, GetAverageCampaignScore\n");
