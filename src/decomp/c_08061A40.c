#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061A40.
 * sub_08061A40 @ 0x08061A40
 */

/* Wave 80 (W80-A). The wave-44 record (W44-H) settled the statement structure
 * -- a 0x10-byte head copy, then a 24-trip loop moving 0xc bytes -- and closed
 * four axes (struct assignment, thumb movstrqi, memcpy/move_by_pieces, and the
 * -mstructure-size-boundary / old_agbcc flag combinations): none of agbcc's
 * block-move paths can emit an unrolled BYTE copy. Its own `next` named the
 * remaining lever: the source wrote the byte moves EXPLICITLY. Explicit byte
 * statements trivially survive -O2 because nothing in gcc 2.95 merges adjacent
 * byte stores, and this draft is that spelling.
 *
 * Three register facts read off the ROM fix it, each measured by probe:
 *   - the sixteen head copies address BOTH parameters in place (r0/r1) with r2
 *     as the byte scratch, so neither parameter may be modified before the
 *     loop: `d += 0x10` copies d out of r0 at entry (`adds r2,r0,#0` hoisted to
 *     the first instruction, r0 becomes the scratch); with `d` untouched and
 *     only `s += 0x10`, it is s that gets copied out instead (`adds r2,r1,#0`).
 *   - the loop pointers are therefore two NEW locals: `dd = d + 0x10` is the
 *     ROM's `adds r2,r0,#0 / adds r2,#0x10`, and `ss = s + 0x10` lands in r1
 *     because s is dead by then, giving the ROM's in-place `adds r1,#0x10`.
 *   - an ascending `for (i = 0; i < 24; i++)` yields the ROM's `movs r3,#0x17 /
 *     subs / cmp #0 / bge` via check_dbra_loop, so the counter's direction is
 *     not readable from the bytes and is left ascending.
 *
 * Whether the original was written this way or as a macro-expanded byte copier
 * cannot be decided from the bytes; the wave-44 comment's worry that an
 * explicit spelling "would be fiction" is noted, but a byte-for-byte match on a
 * shape no compiler block-move path produces is itself the evidence that the
 * source spelled the moves out. */

void CopyAiPersonality(struct Unk085771C4 *dstArg, const struct Unk085771C4 *srcArg)
{
    u8 *d = (u8 *)dstArg;
    const u8 *s = (const u8 *)srcArg;
    u8 *dd;
    const u8 *ss;
    int i;

    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
    d[5] = s[5];
    d[6] = s[6];
    d[7] = s[7];
    d[8] = s[8];
    d[9] = s[9];
    d[10] = s[10];
    d[11] = s[11];
    d[12] = s[12];
    d[13] = s[13];
    d[14] = s[14];
    d[15] = s[15];
    dd = d + 0x10;
    ss = s + 0x10;

    for (i = 0; i < 24; i++)
    {
        dd[0] = ss[0];
        dd[1] = ss[1];
        dd[2] = ss[2];
        dd[3] = ss[3];
        dd[4] = ss[4];
        dd[5] = ss[5];
        dd[6] = ss[6];
        dd[7] = ss[7];
        dd[8] = ss[8];
        dd[9] = ss[9];
        dd[10] = ss[10];
        dd[11] = ss[11];
        dd += 12;
        ss += 12;
    }
}
asm(".global sub_08061A40\n.thumb_set sub_08061A40, CopyAiPersonality\n");
