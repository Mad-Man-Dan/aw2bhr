#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080540F0.
 * sub_080540F0 @ 0x080540F0, sub_0805414C @ 0x0805414C, sub_080541F0 @ 0x080541F0, sub_08054278 @ 0x08054278, sub_080542EC @ 0x080542EC, sub_080543E0 @ 0x080543E0, sub_08054488 @ 0x08054488, sub_08054500 @ 0x08054500
 */

/* Do NOT bind `&gUnknown_02029A10[a1].entries[a2]` to a pointer local. The
 * three-term address `sym + a1*0xb4 + a2*0x24` expands in a different order
 * when it is an explicit local than when it is a CSE of a repeated expression:
 * the local gives `(a1*0xb4) + ((a2*0x24) + sym)`, which needs an extra copy of
 * a1 for the destructive THUMB `mul`, while the repeated expression gives
 * `((a2*0x24) + (a1*0xb4)) + sym` and multiplies in place. Measured on this
 * function in wave 37 (W37-B); the local costs 2 bytes. */
void StartFigureFireScript(u16 a1, u16 a2)
{
    if (gUnknown_02029A10[a1].entries[a2].unk18 != -1
     && IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
        sub_080156E8(gUnknown_02029A10[a1].entries[a2].unk18,
                     gUnknown_02029BA8[a1].unk08);
    SpawnFireEffect(a1, a2);
}
asm(".global sub_080540F0\n.thumb_set sub_080540F0, StartFigureFireScript\n");

/* `row` really is a binding local here and not tidiness: the ROM computes
 * &gUnknown_085D6A48[..] BEFORE the `cmd != 0xff` branch and keeps it in r2
 * across both tests, which only an unconditional statement produces. Binding it
 * is also what keeps the [2] column in the `ldrh` displacement -- spelled
 * inline, agbcc reassociates the column offset onto the symbol instead and
 * spends an extra `adds rB,#4`. */
void StepShotReturnTimeline(u16 a1)
{
    u16 cmd = gUnknown_020296B0[a1].unk0c[gUnknown_02029C08[a1]];
    u16 *row = gUnknown_085D6A48[gUnknown_03004580[a1][1]];

    if (cmd != 0xff
     && gUnknown_03004508 == gUnknown_08551E12[gUnknown_02029C08[a1]] + 0x32
     && row[2] != 1)
    {
        StartFigureReturnScript(a1, cmd);
        gUnknown_02029C08[a1]++;
        gUnknown_02029BEC[a1][cmd] = 0;
        gUnknown_020297C0[a1].unk0c[cmd] = 1;
    }
}
asm(".global sub_0805414C\n.thumb_set sub_0805414C, StepShotReturnTimeline\n");

void StartFigureReturnScript(u16 a1, u16 a2)
{
    PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[a1][1]]
                 .unk08[gUnknown_03004580[a1][3] == 2]);
    if (gUnknown_02029A10[a1].entries[a2].unk18 != -1
     && IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
        sub_080156E8(gUnknown_02029A10[a1].entries[a2].unk18,
                     gUnknown_02029BA8[a1].unk0c);
}
asm(".global sub_080541F0\n.thumb_set sub_080541F0, StartFigureReturnScript\n");

/* Two levers here, both measured in wave 37 (W37-B).
 *
 * `row` must be a LOCAL, not an inline `gUnknown_085D6A48[..][2]`: spelled
 * inline, agbcc reassociates the column offset onto the symbol and spends an
 * extra `adds rB,#4`, where a row pointer in a register leaves the [2] in the
 * `ldrh` displacement. That is the "column-offset fold" split, and it is the
 * same reason gUnknown_03004580's columns read `base + C` in this block.
 *
 * `tbl` exists ONLY to fix the pool order. Of the three address constants the
 * block loads, the ROM creates gUnknown_08553744's first, and RTL creation
 * order is what decides which pool word comes first; the dispatch table is the
 * last thing the expression tree reaches, so without the extra local its word
 * lands third and its `ldr` sinks past the row deref. */
void StepThirdEffectTimeline(u16 a1)
{
    u16 cmd = gUnknown_020296B0[a1].unk0c[gUnknown_020296B0[a1].unk18];

    if (cmd != 0xff
     && gUnknown_03004508
        == gUnknown_020296B0[a1].unk1e[gUnknown_020296B0[a1].unk18])
    {
        void (*const *tbl)(u16, u16) = gUnknown_08553744;
        u16 *row = gUnknown_085D6A48[gUnknown_03004580[a1][1]];

        tbl[row[2]](a1, cmd);
    }
}
asm(".global sub_08054278\n.thumb_set sub_08054278, StepThirdEffectTimeline\n");

/* gUnknown_08136134 and gUnknown_08136138 in the disassembly are NOT globals:
 * they are agbcc's own .rodata address constants, and baserom.gba holds
 * 0x03004580 and 0x020296B0 in them. Naming the two globals directly is the
 * honest spelling and reproduces both pool words. */
void StartFigureThirdScript(u16 a1, u16 a2)
{
    u16 t;

    if (gUnknown_02029A10[a1].entries[a2].unk18 != -1
     && IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
    {
        if (gUnknown_02029A10[a1].entries[a2].unk18 != -1
         && IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
            sub_080156E8(gUnknown_02029A10[a1].entries[a2].unk18,
                         gUnknown_02029BA8[a1].unk10);
        SpawnThirdEffectAndProjectile(a1, a2);
        t = gUnknown_03004580[a1][1];
        if (t == 0xf || t == 0x12 || t == 3)
        {
            PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[a1][1]]
                         .unk0c[gUnknown_03004580[a1][2] - 1]
                               [gUnknown_020296B0[a1].unk1a & 1]);
            gUnknown_020296B0[a1].unk1a++;
        }
        gUnknown_020296B0[a1].unk18++;
    }
}
asm(".global sub_080542EC\n.thumb_set sub_080542EC, StartFigureThirdScript\n");

/* gUnknown_0813613C and gUnknown_08136140 are agbcc .rodata address constants
 * holding 0x03001470 and 0x02029A10 (checked against baserom.gba), not objects
 * in ROM -- so both are spelled as the globals themselves and agbcc rebuilds
 * the pool. */
void StartWholeFigureThirdScript(u16 a1, u16 a2)
{
    if (gUnknown_03001470[gUnknown_02029A10[a1].entries[a2].unk18].unk2c == 1)
        return;
    if (gUnknown_02029A10[a1].entries[a2].unk18 != -1
     && IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
        sub_080156E8(gUnknown_02029A10[a1].entries[a2].unk18,
                     gUnknown_02029BA8[a1].unk10);
    gUnknown_03001470[gUnknown_02029A10[a1].entries[a2].unk18].unk2c = 1;
}
asm(".global sub_080543E0\n.thumb_set sub_080543E0, StartWholeFigureThirdScript\n");

/* The gUnknown_020298E0 twin of StepThirdEffectTimeline: unk1a is this record's command
 * row and unk0c its timestamp row, both walked by the shared unk16 cursor --
 * the same pair gUnknown_020296B0 spells as unk0c / unk1e, with the two offsets
 * swapped. See StepThirdEffectTimeline for why `tbl` and `row` are locals; both
 * assignments sit after the StartBattleHudHpCounter call because that is where the ROM
 * loads the three pool words. */
void StepHitTimeline(u16 a1)
{
    u16 cmd = gUnknown_020298E0[a1].unk1a[gUnknown_020298E0[a1].unk16];

    if (cmd != 0xff
     && gUnknown_03004508
        == gUnknown_020298E0[a1].unk0c[gUnknown_020298E0[a1].unk16])
    {
        void (*const *tbl)(u16, u16);
        u16 *row;

        StartBattleHudHpCounter(a1);
        tbl = gUnknown_0855374C;
        row = gUnknown_085D6A48[gUnknown_03004580[a1][1]];
        tbl[row[2]](a1, cmd);
    }
}
asm(".global sub_08054488\n.thumb_set sub_08054488, StepHitTimeline\n");

void ApplyHitToFigure(u16 a1, u16 a2)
{
    if (gUnknown_02029A10[a1].entries[a2].unk18 != -1
     && IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
    {
        gUnknown_020298E0[a1].unk16++;
        if (gUnknown_08553838[gUnknown_02029A10[a1].entries[a2].unk00 * 2
                              + gUnknown_02029A10[a1].entries[a2].unk01] == 1)
        {
            gUnknown_03004580[a1][5]--;
            gUnknown_03004548[a1] = 1;
            gUnknown_02029A10[a1].entries[a2].unk00 =
                gUnknown_02029A10[a1].entries[a2].unk01;
        }
        SpawnHitEffect(a1, a2);
    }
}
asm(".global sub_08054500\n.thumb_set sub_08054500, ApplyHitToFigure\n");
