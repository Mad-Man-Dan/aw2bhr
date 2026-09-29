#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059C00.
 * sub_08059C00 @ 0x08059C00
 */

/* gUnknown_03003F20 is declared `struct Unk03003338 *` (8-byte records) but
 * this reader walks the same scratch buffer as 4-byte {u8 x; u8 y; s16 v;}
 * cells, the same cast src/decomp/c_0804151C.c makes and the reason the
 * prototype's first parameter is `void *`. See include/unknown-globals.h.
 *
 * `t` is not decoration. `out[0] = 0x270f;` makes agbcc materialise the
 * constant in HImode and then copy it (`ldr r1, =0x270F; add r0, r1, #0;
 * strh r0, [r5]`), 2 bytes over. Routing it through an int keeps the constant
 * in SImode and combine folds the truncation into the `strh`, which is the
 * ROM's single `ldr r0, =0x270F`. `best->v = 0x7fff` needs no such help
 * because CSE hands it the register the comparison already loaded. */
struct Unk59C00Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

s16 AiPopLastNearestCandidate(void *a, u16 *b)
{
    struct Unk59C00Cell *p;
    u16 *out;
    struct Unk59C00Cell *best;
    s16 bv;
    int t;

    p = a;
    out = b;
    bv = 0x7fff;
    best = NULL;
    while (p->v != -1)
    {
        if (p->v <= bv)
        {
            bv = p->v;
            best = p;
        }
        p++;
    }
    if (bv == 0x7fff)
    {
        t = 0x270f;
        out[0] = t;
    }
    else
    {
        best->v = 0x7fff;
        out[0] = best->x;
        out[1] = best->y;
    }
    return bv;
}
asm(".global sub_08059C00\n.thumb_set sub_08059C00, AiPopLastNearestCandidate\n");
