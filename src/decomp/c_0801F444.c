#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F444.
 * sub_0801F444 @ 0x0801F444
 */

/* Sums the tile counts of the gUnknown_0848B780 rows from the a2 row's base
 * tile id (gUnknown_0848B738[a2].unk08) up to a1, and offsets that row's
 * graphics blob by the result -- i.e. where tile a1's graphics start.
 *
 * The `ldrh` at +8 is a (u16) READ of the `int` member, NOT evidence the member
 * is narrow: GetTilePoolFirstGraphic's jump table returns the same six words as `int` and
 * that typing stays. Per the wave-38 brief, a diff that seems to demand a
 * different width for a member someone else already typed is evidence about the
 * ACCESS, so the cast lives here. */
void *GetGraphicSourceAddress(int a1, int a2)
{
    const struct Unk0848B738 *row;
    u8 *base;
    int i;
    u32 total;

    total = 0;
    row = &gUnknown_0848B738[a2];
    i = (u16)row->unk08;
    base = (u8 *)row->unk00;
    for (; i != a1; i++)
        total += gUnknown_0848B780[i].unk00 * gUnknown_0848B780[i].unk01;
    return base + (total & 0x3FF) * 32;
}
asm(".global sub_0801F444\n.thumb_set sub_0801F444, GetGraphicSourceAddress\n");
