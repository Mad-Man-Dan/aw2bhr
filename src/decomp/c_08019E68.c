#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019E68.
 * sub_08019E68 @ 0x08019E68
 */

/* Builds the option list: walks the 0x20-byte entry table until the 0xff
 * terminator, asks each entry's unk04 predicate whether it is available,
 * records every answer in unk24[] and every AVAILABLE entry's index in
 * unk31[], then sizes the window from the widest label and draws it.
 *
 * `w` starts at 0x10 and grows 0x10 per available entry; it reaches
 * DrawWindowBackgroundOnBg2 as `w >> 3`, i.e. in tiles. `maxw` is the widest label in
 * pixels and arrives as `(s16)(maxw + 2)`.
 *
 * The counter `n` is `s16` and that is what the two shift pairs around it are:
 * agbcc's PROMOTE_MODE stores a narrow local zero-extended (`lsls #0x10;
 * lsrs #0x10`) and sign-extends it at each use (`lsls #0x10; asrs #0x10`), so
 * seeing both around one `++` is one variable, not two. `p->unk20` is RELOADED
 * every iteration because the indirect call through unk04 may have moved it. */
void RebuildMenuItems(void)
{
    struct Unk8019A60 *p;
    struct Unk8019A60Item *e;
    s16 n;
    u16 w;
    u16 maxw;
    int i;

    n = 0;
    w = 0x10;
    maxw = 0;

    p = (struct Unk8019A60 *)FindSlotRunningScript(gUnknown_0848A42C);

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

    DrawMenuItems(p);

    DrawWindowBackgroundOnBg2(p->unk48, p->unk4a, (s16)(maxw + 2), w >> 3);
}
asm(".global sub_08019E68\n.thumb_set sub_08019E68, RebuildMenuItems\n");
