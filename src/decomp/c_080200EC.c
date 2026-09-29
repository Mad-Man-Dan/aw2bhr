#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080200EC.
 * sub_080200EC @ 0x080200EC
 */

/* MATCHED, wave 58 (W58-B), first attempt. 244/244 bytes, relocs match.
 *
 * The wave-41 draft was ONE edit away and was stopped by a misreading of the
 * preheader rule. Its note said: "the preheader rule says anything after the
 * `for` init that is loop-invariant was written by the loop optimiser and must
 * NOT be authored -- so `-r` cannot legitimately be spelled as a second loop
 * variable", and then spent three spellings trying to make strength_reduce
 * invent `d` out of `y - cy`. strength_reduce was never going to: `d` is
 * source.
 *
 * READ THE ORDER OF THE TWO INITS, NOT THE RULE'S HEADLINE. In the ROM
 * preheader the `d` init (`rsbs r0,r5,#0; lsrs r6,r0,#0x10`, i.e. `(u16)(-r)`)
 * comes BEFORE the `y` init (`subs r0,r1,r2; lsls; lsrs r4,...`, i.e.
 * `(u16)(cy - r)`). gcc emits a strength_reduce giv init with
 * emit_insn_before(..., loop_start), which places it AFTER the source's own
 * loop-variable initialisation. So an induction variable whose init PRECEDES
 * the counter's init cannot be a giv -- it is source. That is the decidable
 * reverse of the preheader rule and it is the whole finding; see the wave-58
 * chapter in docs/agbcc-codegen.md.
 *
 * `d++` is in the for-INCREMENT, not at the end of the body: the row-bounds
 * `continue` jumps to _080201AA, which carries both increments, so a `d++`
 * written at the end of the body would be skipped by it. y++ is emitted before
 * d++, which fixes the comma order as `y++, d++`.
 *
 * |d| is spelled inline as `d < 0 ? -d : d` at BOTH inner-loop bounds (the
 * wave-41 note said three sites; there are two).
 *
 * The prototype now agrees: `void MapSetInRangeSigned(s16, s16, s16, s16)`. The
 * wave-41 objection to the `int` fourth parameter was right -- the prologue
 * narrows all four arguments -- and has since been applied to the header.
 */

#define MAP gMap

void MapSetInRangeSigned(s16 cx, s16 cy, s16 r, s16 value)
{
    s16 x, y, d;

    d = -r;

    for (y = cy - r; y <= cy + r; y++, d++)
    {
        if (y < 0 || y >= MAP->height)
            continue;
        for (x = cx - r + (d < 0 ? -d : d); x <= cx + r - (d < 0 ? -d : d); x++)
        {
            if (x < 0 || x >= MAP->width)
                continue;
            gUnknown_03003340[y][x] = value;
        }
    }
}
asm(".global sub_080200EC\n.thumb_set sub_080200EC, MapSetInRangeSigned\n");
