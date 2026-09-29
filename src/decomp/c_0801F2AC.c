#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F2AC.
 * sub_0801F2AC @ 0x0801F2AC
 */

/* MATCHED in Wave 61: 160/160 bytes and relocations exact.
 * The historical 100% best.c body was correct but emitted a four-byte
 * out-of-line `inline_fn` (`bx lr` plus alignment), so its object was 164
 * bytes and never matched. Making that helper `static inline` retains the
 * allocation-changing inlining effect while suppressing the extra symbol.
 * The cast on the inner bound is still required to prevent const-driven LICM;
 * the helper spelling on the outer bound is the register-allocation lever. */
static inline struct Unk0848B780 *inline_fn(const struct Unk0848B780 *arg0)
{
    return (struct Unk0848B780 *)arg0;
}

void PutTilePoolGraphicTilemap(int a1, u16 *a2)
{
    struct Unk0200F920 *e;
    int i;
    int x;
    int y;
    int tile;

    e = &gUnknown_0200F920[GetTilePoolForGraphic(a1)];
    for (i = 0; i < e->unk05; i++) {
        if (e->unk08[i].unk02 == a1) {
            tile = e->unk08[i].unk00 | (e->unk04 << 12);
            for (y = 0; y < inline_fn(gUnknown_0848B780)[a1].unk01; y++) {
                for (x = 0; x < ((struct Unk0848B780 *)gUnknown_0848B780)[a1].unk00; x++)
                    a2[x] = tile++;
                a2 += 0x20;
            }
            return;
        }
    }
}
asm(".global sub_0801F2AC\n.thumb_set sub_0801F2AC, PutTilePoolGraphicTilemap\n");
