#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073714.
 * sub_08073714 @ 0x08073714, sub_08073770 @ 0x08073770
 */

/* Bresenham circle rasteriser: walks the octant writing each row's x extent
 * through SetCircleWindowEdge into the gUnknown_0202F8DC scanline table, twice per
 * step -- (x, y) and (y, x).
 *
 * TWO GIV LEVERS, and the function is exactly the interaction between them
 * (wave 46, W46-H; the previous analysis was right about the question and had
 * found only half the answer):
 *
 *  - `t` exists so that `x * 2 - 2` becomes a giv at all. Written inline as
 *    `d += x * 2 - 2;`, fold reassociates it to `(d - 2) + x * 2` and
 *    strength_reduce never sees a two-operation giv, so the ROM's preheader
 *    `lsls r0,r4,#1; subs r7,r0,#2` and its `subs r7,#2` update never appear.
 *
 *  - `u` exists to STOP the matching thing happening on the y side. Written
 *    inline as `d = d + 1 - y * 2;`, fold reassociates it to `d - (y * 2 - 1)`,
 *    which IS a two-operation giv, so agbcc reduces that one too and the loop
 *    carries a sixth value (+12 bytes: an extra callee-saved register pushed,
 *    and `mov r0,#1; neg r0,r0; mov r8,r0` in the preheader). Binding `d + 1`
 *    to its own local blocks the reassociation, leaving a bare `y * 2` whose
 *    benefit is one shift -- below strength_reduce's worth-while threshold --
 *    so it stays inline as `adds r1,r5,#1; lsls r0,r6,#1; subs r5,r1,r0`.
 *
 * So the two `* 2` terms are NOT competing for one register, which is how this
 * read for a wave: they are independent fold decisions, and the source has to
 * take opposite sides on them. Same lever, opposite polarity. */

#include "global.h"

#include "hardware.h"
#include "proc.h"
/* The HBlank window-table teardown, and ScanlineDarkenBg0_Init's opposite number: it
 * re-points the gUnknown_0202FDE4 slot at gUnknown_0202F8DC and fills all 0xA0
 * scanlines with 0 rather than 0x10.  The fill re-loads the pointer from the
 * global on every pass for the reason recorded in include/unknown-globals.h --
 * the halfword store may alias the pointer variable, so only the ADDRESS is
 * hoisted (r4).  Writing the cast at the point of use reproduces that; a typed
 * local bound before the loop hoists and loses 4 bytes.
 *
 * The proc is CircleWipe_Loop's, whose unk58 is the frame counter this resets.
 *
 * `movs #0x21; rsbs` is the single-bit bitfield tell (a scalar `&= ~0x20`
 * emits a bare `movs #0xdf`), and the bit is byte 1 of gDispIo.disp_ct, i.e.
 * DISPCNT bit 13 = win0_enable.
 *
 * The two blend-shadow stores are byte 1 -- `ldrb [r1, #1]` -- so they are
 * win1_enable_blend, NOT the win0_enable_blend of the same-shaped tail in
 * src/decomp/c_0807F378.c, which loads at displacement 0. */

struct Unk08073770Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void BuildCircleWindowTable(int a1)
{
    int x;
    int y;
    int d;
    int u;

    d = a1;

    gUnknown_0202FDE4 = gUnknown_0202F8DC;

    x = a1;
    y = 0;

    while (x >= y)
    {
        SetCircleWindowEdge(x, y >> 1, gUnknown_0202FDE4);
        SetCircleWindowEdge(y, x >> 1, gUnknown_0202FDE4);

        u = d + 1;
        d = u - y * 2;

        if (d < 0)
        {
            int t = x * 2 - 2;

            d += t;
            x--;
        }

        y++;
    }
}
asm(".global sub_08073714\n.thumb_set sub_08073714, BuildCircleWindowTable\n");

void CircleWipe_Init(struct Unk08073770Proc *proc)
{
    int i;

    gUnknown_0202FDE4 = gUnknown_0202F8DC;

    for (i = 0; i < 0xA0; i++)
        ((u16 *)gUnknown_0202FDE4)[i] = 0;

    proc->unk58 = 0;

    gDispIo.disp_ct.win0_enable = 0;

    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0;
    gUnknown_030024E4 = 0;
    gUnknown_030020B8 = 0xA0;

    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;
}
asm(".global sub_08073770\n.thumb_set sub_08073770, CircleWipe_Init\n");
