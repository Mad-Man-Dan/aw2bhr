#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E584.
 * sub_0804E584 @ 0x0804E584
 */

#include "hardware.h"
/* Third member of the CruiserFigure_Init / BattleshipFigure_Init family (both matched, both
 * 600 B) -- same subsystem, same `pos` reseed out of GetFigurePositionTable, same
 * `entry->x - *ox` tail. The two levers that closed those two do NOT transfer,
 * and that was measured rather than assumed (see docs/agbcc-codegen.md):
 * the wave-17 `(meta = gUnknown_03004580, ...)` comma anchor is BYTE-NEUTRAL
 * here, and the zero-trip `do { } while (0)` around the GetFigurePositionTable call is a
 * REGRESSION. What does transfer is the `(e1 = &pos[..][..])->x` binding pair,
 * which is worth 3 instructions and takes the draft from 124 differing
 * instruction lines to 21.
 *
 * `int x0` is the last lever and it is the interesting one. The ROM
 * sign-extends `v` (`lsls #16; asrs #16`) before adding it to unk04, even
 * though the sum is immediately stored with `strh` and the extension is
 * therefore dead. Every cast spelling on the OPERAND -- `(s16)v` with `v`
 * declared `s16`, `u16` or `int` -- is byte-identical and folds, because
 * combine's force_to_mode sees the 16-bit store and drops the extension. What
 * stops it is binding the SUM to an `int`: the addition then has a 32-bit
 * consumer, the extension is no longer redundant, and it survives. `x0` is
 * never read. Found by decomp-permuter at iteration 5018 after four hand
 * rounds had failed on it, and it is the same "bind at the right depth" family
 * as the two levers above -- here the depth is the result, not an operand.
 *
 * `pos` rows are 40 bytes because GetFigurePositionTable hands back a 2-D view:
 * `pos[gUnknown_03004580[i][5]][i * 5 + j]` of 4-byte (x, y) pairs, the shape
 * data/parked.json records for sub_0804FA2C. The gUnknown_03004504 guard is
 * wave 19's nested-bitfield rule -- two separate mask tests, not an `&&`.
 * Promotion must carry rodata: ["0x08136080", "0x08136084", "0x08136088",
 * "0x0813608C"], the -fforce-addr copies of &gUnknown_03001FBC,
 * &gUnknown_0300453C, &gUnknown_0300451C and &gUnknown_03004580. */

struct UnkPosPair
{
    u16 x;
    u16 y;
};
void SetFigureObjAttrs(u16, u16, s16);

void GroundFigure_Init(void)
{
    int x0;
    struct UnkPosPair (*pos)[10];
    u16 *ox;
    u16 *oy;
    s16 v;
    struct UnkPosPair *e1;
    struct UnkPosPair *e2;

    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    SetFigureObjAttrs(gUnknown_0300453C, gUnknown_0300451C, gUnknown_03001FBC);

    pos = (struct UnkPosPair (*)[10])GetFigurePositionTable(
        gUnknown_085D6A48[gUnknown_03004580[gUnknown_0300453C][1]][0],
        gUnknown_03004580[gUnknown_0300453C][3]);

    v = gUnknown_085644E0[gUnknown_0300453C][gUnknown_0300450C];

    if (gUnknown_03004504.bit0)
    {
        if (!gUnknown_03004504.bit6)
            v = 0;
    }

    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 =
        (e1 = &pos[gUnknown_03004580[gUnknown_0300453C][5]]
                  [gUnknown_0300453C * 5 + gUnknown_0300451C])->x;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x =
        (x0 = v + gUnknown_02029A10[gUnknown_0300453C]
                      .entries[gUnknown_0300451C].unk04);
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 =
        (e2 = &pos[gUnknown_03004580[gUnknown_0300453C][5]]
                  [gUnknown_0300453C * 5 + gUnknown_0300451C])->y;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06;

    StartFigureEntrySlide(gUnknown_0300453C, gUnknown_0300451C,
                 gUnknown_0855214C[gUnknown_0300453C], 0x32);

    ox = gUnknown_084C3F70[gUnknown_0300453C];
    oy = gUnknown_084C3F78[gUnknown_0300453C];
    SetSlotSpritePosition(gUnknown_03001FBC,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x - *ox,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y - *oy);

    SetSlotSpriteHook(gUnknown_03001FBC,
        gUnknown_08552FB8[gUnknown_03004580[gUnknown_0300453C][1]]
                         [gUnknown_03004580[gUnknown_0300453C][2]][2]);
}
asm(".global sub_0804E584\n.thumb_set sub_0804E584, GroundFigure_Init\n");
