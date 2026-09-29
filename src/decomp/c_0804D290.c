#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D290.
 * CruiserFigure_Init @ 0x0804D290
 */

/* MATCHED -- byte-for-byte identical to the original.
 *
 * Rebuilds the OBJ attributes of the current gUnknown_03001470 slot, reseeds
 * the gUnknown_02029A10 entry's position from the ROM table GetFigurePositionTable hands
 * back, then arms a continuation out of gUnknown_08552FB8. Same family as
 * CruiserPart2_Init / BattleshipPart2_Init / WholeFigure_Init -- see those for the `* 0x100`,
 * the `pal`/`prio` temporaries and the `(e1 = &pos[...])` binding.
 *
 * TWO of the three constructs below are levers, not natural source, and both
 * are load-bearing. Removing either costs 12 bytes and takes the function from
 * exact to 588 bytes -- SMALLER than the ROM, which is the tell.
 *
 * 1. `ox` / `oy`. The ROM materialises BOTH camera-origin pointers, one of them
 *    into `ip`, before it computes even the first argument of SetSlotSpritePosition.
 *    Only a pair of locals forces that order.
 *
 * 2. `do { } while (0)`. The wave-16 allocation lever: allocno_compare weights
 *    every reference by loop_depth, so a zero-trip loop promotes the pseudos
 *    inside it. Without it the 0xb4 gUnknown_02029A10 group stride (six
 *    references) takes r6 and &gUnknown_03004580 (two) is pushed to sl, which
 *    saves a `mov` at each of five `muls` -- in the ROM's favour.
 *
 * 3. `(meta = gUnknown_03004580, ...)`. This one is the whole last 22 bytes and
 *    it is a POSITION, not a value: `meta` is never read. Of two address
 *    constants each used twice in the same two statements, agbcc gives the
 *    callee-saved register to whichever pseudo is created FIRST, and emits that
 *    one's `ldr` at the head of the statement while the loser is rematerialised
 *    at each use. The ROM's pool order is 085D6A48, 03004580, 03004582, so
 *    &gUnknown_03004580's pseudo must exist AFTER gUnknown_085D6A48's outer base
 *    and BEFORE gUnknown_03004582's inner one -- a point inside argument 1 that
 *    no statement boundary can reach. The two neighbouring spellings each miss
 *    by exactly one slot: with no anchor the `ldr` lands twelve instructions
 *    late (96.3%), and with `meta = gUnknown_03004580;` as its own statement
 *    ahead of the call it lands one instruction early and swaps the same two
 *    pool words the other way (99.7%, two bytes).
 *
 * Ruled out by measurement: old_agbcc (624 B), -fno-force-addr (bit-identical
 * output, so these `ldr`s are not a -fforce-addr artefact), a `u16 (*)[8]`
 * local for the reads, `*(g[i] + 3)`, `(&g[0][3])[i * 8]`, a struct row for
 * 085D6A48 column 0, and 300 s of decomp-permuter.
 */

struct UnkPosPair
{
    u16 x;
    u16 y;
};

void CruiserFigure_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;
    struct UnkPosPair *pos;
    struct UnkPosPair *e1;
    struct UnkPosPair *e2;
    u16 (*meta)[8];
    u16 *ox;
    u16 *oy;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;

    do
    {
        pos = (struct UnkPosPair *)GetFigurePositionTable(
            gUnknown_085D6A48[(meta = gUnknown_03004580,
                gUnknown_03004582[gUnknown_0300453C][0])][0],
            gUnknown_03004580[gUnknown_0300453C][3]);
    } while (0);

    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 =
        (e1 = &pos[gUnknown_0300453C * 5 + gUnknown_0300451C])->x;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 =
        (e2 = &pos[gUnknown_0300453C * 5 + gUnknown_0300451C])->y;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk1e = 0;

    StartFigureEntrySlide(gUnknown_0300453C, 0, 0, 0);

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);

    ox = gUnknown_084C3F70[gUnknown_0300453C];
    oy = gUnknown_084C3F78[gUnknown_0300453C];
    SetSlotSpritePosition(gUnknown_03001FBC,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x - *ox,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y - *oy);

    SetSlotSpriteHook(gUnknown_03001FBC,
        gUnknown_08552FB8[gUnknown_03004582[gUnknown_0300453C][0]]
                         [gUnknown_03004580[gUnknown_0300453C][2]][2]);
}

asm(".global sub_0804D290\n.thumb_set sub_0804D290, CruiserFigure_Init\n");
