#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080878A8.
 * sub_080878A8 @ 0x080878A8, sub_08087938 @ 0x08087938
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* Lays out one PutCoMinimugSprite sprite per unit past the first, right-to-left from
 * x = 0xd8, then draws two fixed DrawOamObject sprites.
 *
 * All THREE loop quantities -- the x coordinate, the 0xc-stride tile index and
 * the 0x1000-stride palette field -- are strength-reduction GIVs, not source
 * variables: their inits sit in the preheader AFTER the `bge` entry guard,
 * where only the loop optimiser writes. Authoring them as accumulators puts
 * the inits before the guard and additionally parks the 0x1000 constant in a
 * callee-saved register the ROM rematerialises, which costs 2 bytes. Only
 * `(n - 2) * 0x20` is a source-level invariant, and it is the one thing LICM
 * hoists ahead of those inits.
 *
 * The OR operand order in the third argument decides which giv gets the high
 * register: with the palette term written first, the tile giv lands in r8 and
 * its `+ 0xc` step fits an imm8 `add r8, r0`, which is the ROM's encoding. */

struct Unk080878A8Proc
{
    PROC_HEADER;
    STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
};

void EnemyCoMinimugs_Loop(struct Unk080878A8Proc *proc)
{
    int n;
    int i;

    n = GetMapArmyCount(gUnknown_02027F74.unk04[proc->unk54]);

    for (i = 0; i < n - 1; i++)
        PutCoMinimugSprite(0x1000 | (0xd8 - (n - 2) * 0x20 + i * 0x20), 0xa0,
            0x400 | (0x7000 + i * 0x1000) | (0x90 + i * 0xc), 6);

    DrawOamObject(0x69, 0x26, 0x98, 0, 2);
    DrawOamObject(0x52, 0xd0, 0x88, 0, 2);
}

/* Three sequential LoadTilePoolGraphic runs over four id ranges: two singletons, then
 * a paired loop over 0x79.. and 0x6f.. and a single loop over 0x6a... The two
 * biased arguments in the first loop are separate expressions in the counter,
 * not one induction variable. No data refs at all. */

void LoadMapSelectPanelGraphics(void)
{
    int i;

    LoadTilePoolGraphic(0x61);
    LoadTilePoolGraphic(0x60);

    for (i = 0; i < 10; i++)
    {
        LoadTilePoolGraphic(i + 0x79);
        LoadTilePoolGraphic(i + 0x6f);
    }

    for (i = 0; i < 5; i++)
        LoadTilePoolGraphic(i + 0x6a);
}
asm(".global sub_08087938\n.thumb_set sub_08087938, LoadMapSelectPanelGraphics\n");

asm(".global sub_080878A8\n.thumb_set sub_080878A8, EnemyCoMinimugs_Loop\n");
