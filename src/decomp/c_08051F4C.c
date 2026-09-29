#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051F4C.
 * sub_08051F4C @ 0x08051F4C
 */

/* MATCHED. Not part of the SmokeEffect_Init cluster -- a fresh function in the
 * same neighbourhood that reuses its type vocabulary. It installs a unit's
 * sprite for one side/slot pair: reset the two per-side scratch words, publish
 * the side and slot into gUnknown_0300453C / gUnknown_0300451C, allocate the
 * sprite through sub_08015410 and stash its id, then set five of its
 * attributes and decide which follow-up runs.
 *
 * Three spellings had to be found:
 *
 *  - The FIFTH argument of sub_08015410 is its own statement (`f`). Inline, the
 *    whole `gUnknown_08552178[a][b] * 7 + 6` expression is evaluated after the
 *    other four arguments and needs an extra `ip` shuffle; the ROM computes it
 *    before the two pool words and only narrows it to u8 at the call, which is
 *    what a plain local produces. `int f`, not `u16` -- a u8-narrowing local
 *    would move the `lsls #0x18; lsrs #0x18` up to the assignment.
 *
 *  - `row` is load-bearing. `gUnknown_085D6A48[w][2]` folds the constant 4
 *    into the symbol's address (`adds r2,#4`) where the ROM wants it in the
 *    `ldrh` immediate; binding the row to a `u16 *` and subscripting that
 *    gives `ldrh [r0,#4]`. Same rule as the struct-vs-array choice on
 *    gUnknown_08552D80 in SmokeEffect_Init, reached from the other direction --
 *    here the shared declaration is an array and the local supplies the
 *    aggregate.
 *
 *  - `c` and `d` are real variables, not a spelling artefact. The parameters
 *    live in r7/sb for the body and the copies live in [sp,#4]/sl purely so
 *    the LAST StartFigureFall call can use them; `StartFigureFall(a, b)` there
 *    coalesces the copies away and loses two instructions. Same
 *    two-variables-two-registers rule as sub_0800C124 in
 *    docs/agbcc-codegen.md.
 *
 * `entries[b].unk00 = 1` is measured, and its sibling DeathHandler_Tank needs
 * `= unk01` for the identical-looking statement -- see the note there. */
void DeathHandler_Air(u16 a, u16 b)
{
    u16 c;
    u16 d;
    u16 e;
    int f;
    u16 *row;

    c = a;
    d = b;

    gUnknown_02028E5C[a][0] = 1;
    *gUnknown_084C3F78[a] = 0;
    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    f = gUnknown_08552178[a][b] * 7 + 6;

    gUnknown_02029808[a].unk24[b] = sub_08015410(gUnknown_085536BC, 1,
        gUnknown_02029808[a].unk44[gUnknown_02029808[a].unk2e],
        gUnknown_02029808[a].unk58[gUnknown_02029808[a].unk2e],
        f);

    PlayFigureDestroySfx(a, 0x10);

    e = gUnknown_08553B14[a];

    EnableSlotSpriteAffine(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteDoubleSize(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
    SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x100);

    row = gUnknown_085D6A48[gUnknown_03004580[a][1]];

    if (row[2] == 1)
    {
        if (b == gUnknown_08552148[a] && gUnknown_03004580[a][6] == 0)
            StartFigureFall(a, b);
    }
    else if (gUnknown_02029A10[a].entries[b].unk01 == 1)
    {
        gUnknown_02029A10[a].entries[b].unk00 = 1;
    }
    else if (gUnknown_02029A10[a].entries[b].unk01 == 0)
    {
        StartFigureFall(c, d);
    }
}
asm(".global sub_08051F4C\n.thumb_set sub_08051F4C, DeathHandler_Air\n");
