#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B9C8.
 * sub_0801B9C8 @ 0x0801B9C8
 */

/* One line of the gUnknown_0808EF64 text stream: CacheTextString resolves the
 * cursor to a slot (-1 when there is none), the slot's 4-byte record supplies
 * an x offset and a byte, and PutTextTileRows paints with them. The return value
 * is sub_0808B6B0's, i.e. how far the cursor advanced.
 *
 * TWO measured facts hold this together.
 *   (1) `p` must be BOUND BEFORE the call. The base address is live across
 * `bl CacheTextString` in a callee-saved register -- that is the fourth entry in
 * the push list -- and referencing gUnknown_03002B80 only inside the argument
 * list creates the pseudo after the call, giving `push {r4,r5,r6}` and a
 * 4-byte-short function. Read the push list first.
 *   (2) The record array must be a MEMBER of a struct wrapping the whole
 * block, not a `struct Ent *` pointed at 0x03002DD8. Both spell the same
 * layout, but `p->unk258[i]` keeps 0x258 as a runtime add off the bare symbol
 * (`movs r1,#0x96; lsls #2`) with `&g + i*4` CSEd across the two members,
 * where `q[i].m` on a pre-offset pointer folds 0x258 into the pool word and
 * reaches the second member by displacement. The ROM has the first shape.
 *
 * Parameters 3 and 4 are int, not u16 -- see the note in
 * include/unknown-functions.h; the prologue has no PROMOTE_MODE shift pair. */
struct Unk1B9C8Ent
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 filler_03[0x01];
};
struct Unk1B9C8Blk /* the gUnknown_03002B80 block seen by this function */
{
    /* 0x000 */ u8 filler_000[0x258];
    /* 0x258 */ struct Unk1B9C8Ent unk258[0x40];
    /* 0x358 */ u16 unk358;
};

int PutCachedTextString(int a, u32 b, int c, int d)
{
    struct Unk1B9C8Blk *p = (struct Unk1B9C8Blk *)&gUnknown_03002B80;
    int i;

    i = CacheTextString((const char *)b, d);
    if (i == -1)
        return 0;

    PutTextTileRows((void *)a, p->unk258[i].unk00 + c, p->unk258[i].unk02);
    return sub_0808B6B0((const char *)b);
}
asm(".global sub_0801B9C8\n.thumb_set sub_0801B9C8, PutCachedTextString\n");
