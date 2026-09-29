#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019F90.
 * sub_08019F90 @ 0x08019F90
 */

/*
 * CreateMenu -- build an option list and return it.
 *
 * `a1` is an array of 0x20-byte item records, ending at the one whose .unk00 is
 * 0xff. `a2` and `a3` are the list's top-left cell, `a4` chooses which teardown
 * the list gets and `a5` is the row the cursor starts on.
 *
 *   1. sub_080152EC puts the gUnknown_0848A42C script in a gUnknown_03001470
 *      slot; that slot, seen as struct Unk8019A60, is the list object. Its
 *      .unk0c takes Menu_OnEndBlankBg2 when a4 is 0 and Menu_OnEndReleaseMapLock when it is 1.
 *   2. Walk the item array. Each item's .unk04 is called and its result kept as
 *      that item's flags in .unk24. An item with bit 0 clear is visible: its
 *      index is appended to the row table .unk31, and the pixel width of its
 *      string (gTextTable[.unk1c], measured by GetStringWidthInTiles) goes into the
 *      running maximum. Each visible row adds 0x10 to a height counter that
 *      starts at 0x10.
 *   3. Record the item count, the visible-row count and the starting cursor
 *      row, then take a second slot for the cursor sprite. It goes in .unk44,
 *      sized from a2 and a3 in pixels, with the cursor row in its .unk20.
 *   4. DrawMenuItems draws the rows, and DrawWindowBackgroundOnBg2 opens the window round
 *      them: the same position, the widest string plus 2, and the height
 *      counter divided by 8.
 *
 * Returns the list object as an int. src/decomp/c_08019E68.c rebuilds an
 * existing list with the same walk; read it alongside this one.
 *
 * `p->unk31[13] = 0` writes at +0x3e, which falls inside .unk31. 13 is very
 * likely one past the row table's real end -- see the note on struct
 * Unk8019A60 in include/unknown-globals.h. Both readings emit the same bytes,
 * so the shared member is left alone.
 *
 * Why the C looks odd: the height is divided as `(w >> 2) >> 1` and not
 * `w >> 3`. The extra shift is one more instruction at the point where the
 * compiler decides which stack slots to spill to; with the single shift, the
 * eight values it has hoisted out of the loop land in each other's slots and 17
 * bytes of stack offsets differ. Leave the two shifts.
 */

int CreateMenu(const void *a1, u16 a2, u16 a3, u16 a4, u16 a5)
{
    struct Unk8019A60 *p;
    struct Unk8019A60Item *e;
    struct Unk03001470 *q;
    s16 n;
    u16 w;
    u16 maxw;
    int i;

    n = 0;
    w = 0x10;
    maxw = 0;

    p = (struct Unk8019A60 *)sub_080152EC(gUnknown_0848A42C, 0);
    p->unk4c = a4;

    switch (a4)
    {
    case 0:
        p->unk0c = Menu_OnEndBlankBg2;
        break;

    case 1:
        p->unk0c = Menu_OnEndReleaseMapLock;
        break;
    }

    p->unk20 = (struct Unk8019A60Item *)a1;

    for (i = 0, e = p->unk20; e->unk00 != 0xff; i++, e = &p->unk20[i])
    {
        int r;
        u16 t;

        r = e->unk04();
        p->unk24[i] = r;

        if ((r & 1) == 0)
        {
            p->unk31[n++] = i;

            t = GetStringWidthInTiles((const char *)gTextTable[e->unk1c]);

            if (maxw < t)
                maxw = t;

            w += 0x10;
        }
    }

    p->unk40 = i;
    p->unk41 = n;
    p->unk43 = a5;
    p->unk42 = a5;
    p->unk31[13] = 0;
    p->unk48 = a2;
    p->unk4a = a3;

    q = sub_080152EC(gUnknown_0848A44C, 0);
    p->unk44 = q;
    q->unk24 = (a2 + 1) * 8;
    q->unk26 = (a3 + 1) * 8;
    q->unk20 = a5;

    DrawMenuItems(p);

    DrawWindowBackgroundOnBg2(p->unk48, p->unk4a, (s16)(maxw + 2), (w >> 2) >> 1);

    return (int)p;
}
asm(".global sub_08019F90\n.thumb_set sub_08019F90, CreateMenu\n");
