#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019C40.
 * sub_08019C40 @ 0x08019C40
 */

/*
 * DrawMenuItems -- draw an option list's rows into the BG0 tilemap.
 *
 * `p` is the list object. .unk41 is the number of visible rows, .unk31 maps a
 * row to an item index, .unk24 holds one flags byte per item, and .unk20 is the
 * item array, whose .unk1c is the item's string id. Two globals are cleared,
 * ClearBg0TilemapBuffer goes over the tilemap buffer, and then for each row
 * PutTextTableEntryImmediate draws the string at cell (.unk48 + 1, .unk4a + row * 2 + 1) in
 * gBG0TilemapBuffer. Its last argument is 1 when bit 1 of the item's flags is
 * set and 0 otherwise; what PutTextTableEntryImmediate does with it is not visible here. The
 * whole 0x800-byte buffer then goes to BG VRAM at 0x06007000.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The row coordinate is grouped `p->unk4a + (i * 2 + 1)`. Written
 *     `p->unk4a + 1 + i * 2` the compiler regroups the sum and folds the
 *     constant onto the multiply.
 *   - The flag is negated in a statement of its own, `neg = -flag;`, with
 *     `flag` a u8 local. In one expression the negation and the shift come out
 *     next to each other, where the original has them thirteen instructions
 *     apart.
 *   - The two clears are one chained assignment. As two statements the compiler
 *     folds the second read to a constant, and gUnknown_03001418 being volatile
 *     is what makes it re-read the halfword it has just stored.
 *   - `i` is s16, which is why the index arithmetic is recomputed every pass
 *     instead of being carried in a register.
 */
void DrawMenuItems(struct Unk8019A60 *p)
{
    s16 i;

    gUnknown_03001FF8 = gUnknown_03001418 = 0;

    ClearBg0TilemapBuffer();

    for (i = 0; i < p->unk41; i++)
    {
        u8 k;
        u8 flag;
        int neg;

        k = p->unk31[i];
        flag = p->unk24[k] & 2;
        neg = -flag;

        PutTextTableEntryImmediate((s16)(p->unk48 + 1),
                     (s16)(p->unk4a + (i * 2 + 1)),
                     gBG0TilemapBuffer,
                     p->unk20[k].unk1c,
                     0x8000,
                     (u32)neg >> 31);
    }

    RegisterDataMove(gBG0TilemapBuffer, (void *)0x06007000, 0x800);
}
asm(".global sub_08019C40\n.thumb_set sub_08019C40, DrawMenuItems\n");
