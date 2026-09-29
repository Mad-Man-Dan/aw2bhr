#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BDD8.
 * sub_0804BDD8 @ 0x0804BDD8, sub_0804BECC @ 0x0804BECC
 */

/* Advances one (side, slot) animation frame and rewrites the slot's OBJ
 * palette. Byte-identical twin of StepFigureHitFlash2 -- both read the SAME global,
 * gUnknown_020298E0; the differing pool symbols (0x0813604C vs 0x08136050)
 * are two private -fforce-addr .rodata copies of the address 0x020298E0, so
 * one source spells both.
 *
 * unk30 is the frame accumulator, unk26 the per-frame step and unk3a a signed
 * countdown; on the frame the accumulator reaches 4 the countdown ticks and
 * both it running out and the OTHER side's gUnknown_02029BE8 cell going zero
 * stop the step.
 *
 * The final sum is written `unk26 + unk30` and not the other way round: the
 * two member addresses are materialised in source order, and the ROM adds
 * 0x26 to the base first.
 */
int StepFigureHitFlash(u16 a, u16 b, s16 c)
{
    struct OamData oam;
    u16 pal;
    u16 t;

    CopySlotSpriteAttrs(c, (struct UnkVec *)&oam);

    gUnknown_020298E0[a].unk30[b] += gUnknown_020298E0[a].unk26[b];

    if (gUnknown_020298E0[a].unk30[b] == 4)
    {
        if (--gUnknown_020298E0[a].unk3a[b] <= 0)
            gUnknown_020298E0[a].unk26[b] = 0;

        if (gUnknown_02029BE8[a ^ 1] == 0)
            gUnknown_020298E0[a].unk26[b] = 0;

        gUnknown_020298E0[a].unk30[b] = 0;
    }

    t = gUnknown_020298E0[a].unk26[b] + gUnknown_020298E0[a].unk30[b];
    pal = gUnknown_08553B40[a][t];
    oam.paletteNum = pal;
    SetSlotSpriteAttrs(c, *(struct UnkVec *)&oam);

    return t;
}
asm(".global sub_0804BDD8\n.thumb_set sub_0804BDD8, StepFigureHitFlash\n");

/* Byte-identical twin of StepFigureHitFlash -- SAME SOURCE. Both read
 * gUnknown_020298E0; the differing pool symbol (0x08136050 here, 0x0813604C
 * there) is a second private -fforce-addr .rodata copy of 0x020298E0. */
int StepFigureHitFlash2(u16 a, u16 b, s16 c)
{
    struct OamData oam;
    u16 pal;
    u16 t;

    CopySlotSpriteAttrs(c, (struct UnkVec *)&oam);

    gUnknown_020298E0[a].unk30[b] += gUnknown_020298E0[a].unk26[b];

    if (gUnknown_020298E0[a].unk30[b] == 4)
    {
        if (--gUnknown_020298E0[a].unk3a[b] <= 0)
            gUnknown_020298E0[a].unk26[b] = 0;

        if (gUnknown_02029BE8[a ^ 1] == 0)
            gUnknown_020298E0[a].unk26[b] = 0;

        gUnknown_020298E0[a].unk30[b] = 0;
    }

    t = gUnknown_020298E0[a].unk26[b] + gUnknown_020298E0[a].unk30[b];
    pal = gUnknown_08553B40[a][t];
    oam.paletteNum = pal;
    SetSlotSpriteAttrs(c, *(struct UnkVec *)&oam);

    return t;
}
asm(".global sub_0804BECC\n.thumb_set sub_0804BECC, StepFigureHitFlash2\n");
