#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B2A8.
 * sub_0804B2A8 @ 0x0804B2A8, sub_0804B330 @ 0x0804B330
 */

/* The tracking half of the ~40 sprite setters between 0x0804B180 and
 * 0x08053614: it re-points the proc's own sprite at its
 * gUnknown_02029A10 entry, in coordinates relative to the side's two scroll
 * origins, and does nothing while the side's record is disarmed.
 *
 * THREE BOUND LOCALS AND THE ORDER THEY IMPOSE ARE THE WHOLE PROBLEM HERE.
 * With the two subtractions written inline the ROM's `ldrh` of the entry's `y`
 * and the second scroll pointer both drift past the first subtraction, because
 * agbcc emits each argument's loads at that argument. Naming `y`, `q1` and `q2`
 * in statements of their own puts all three ahead of both subtractions, which
 * is what the ROM does -- and the entry address is CSEd across the `y` and `x`
 * reads, so it survives as one base with #0xa / #8 displacements.
 *
 * unk30/unk34 are `u32` members (wave 26) read `ldrh`: a truncation at the use,
 * not evidence for a halfword member -- the same narrowing agbcc does for
 * gUnknown_0200C528's unk14 in c_08018758.
 *
 * gUnknown_03001FBC is named three times and loaded once (`ldrsh` through the
 * register-offset form), which is CSE and not a local. */
void FigureSprite_Loop(void)
{
    u16 g;
    u16 s;
    u16 y;
    u16 *q1;
    u16 *q2;

    g = gUnknown_03001470[gUnknown_03001FBC].unk30;
    s = gUnknown_03001470[gUnknown_03001FBC].unk34;

    if (gUnknown_02029690[g].unk08 != 1 && gUnknown_02029690[g].unk00 != 0)
    {
        y = gUnknown_02029A10[g].entries[s].y;
        q1 = gUnknown_084C3F70[g];
        q2 = gUnknown_084C3F78[g];
        SetSlotSpritePosition(gUnknown_03001FBC,
                     gUnknown_02029A10[g].entries[s].x - *q1, y - *q2);
    }
}
asm(".global sub_0804B2A8\n.thumb_set sub_0804B2A8, FigureSprite_Loop\n");

/* The per-frame step of one side's shake/bounce animation. It bumps the
 * record's counter, decides whether the animation is over, then adds this
 * frame's (x, y) offset out of the record's own table into the two scroll
 * origins.
 *
 * THE THREE POINTER LOADS AT THE END ARE BOUND LOCALS, and that is the whole
 * difference between this and a candidate that is otherwise instruction-exact:
 * written inline, agbcc interleaves each `ldr` with the statement that uses it
 * (q1, then the table, then q2 after the first `strh`). The ROM loads all three
 * up front, which is what a statement of its own for each produces.
 *
 * `i = 0` is reached from BOTH sides of the bit0 test -- the `b` out of the
 * then-arm lands on the else-arm's own `movs r6, #0`, which is tail merging,
 * not a shared statement.
 *
 * `d = -d` on a `u16` is `rsbs` followed by `lsls #0x10; lsrs #0x10`: the
 * truncation is part of the assignment, not a separate cast.
 *
 * gUnknown_08136030 in the disassembly is agbcc's own -fforce-addr address
 * constant for gUnknown_02029690 (baserom.gba has 0x02029690 in that word), so
 * the array is named directly and the pool word is placed by the build. */
void StepPanelScrollDriver(u16 a)
{
    u16 i;
    u16 d;
    u16 *p;
    u16 *q1;
    u16 *q2;

    gUnknown_02029690[a].unk0e++;
    i = gUnknown_02029690[a].unk0e;
    d = 0;

    if (gUnknown_03004504.bit0)
    {
        gUnknown_02029690[a].unk08 = 1;
        i = 0;
    }
    else
    {
        if (i == 0x28)
            gUnknown_02029690[a].unk08 = 1;
        if (gUnknown_02029690[a].unk08 == 1)
            i = 0;
    }

    if (gUnknown_02029690[a].unk00 == 2)
    {
        d = gUnknown_02029690[a].unk0e & 1;
        if (a != 0)
            d = -d;
    }

    p = gUnknown_02029690[a].unk04;
    q1 = gUnknown_084C3F70[a];
    q2 = gUnknown_084C3F78[a];
    *q1 = *q1 + d + p[i];
    *q2 += p[i + 0x28];
}
asm(".global sub_0804B330\n.thumb_set sub_0804B330, StepPanelScrollDriver\n");
