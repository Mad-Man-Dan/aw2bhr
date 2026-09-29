#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804DF00.
 * sub_0804DF00 @ 0x0804DF00
 */

/* SubmarineFigure_Loop's twin one step further into the same script, and the same
 * preamble src/decomp/c_0804D0FC.c uses -- including the `u16 t` for
 * sub_080156C4's `int` result, which is where the `lsls #0x10; lsrs #0x10`
 * before the `cmp r0,#2` comes from. gUnknown_08136078 and gUnknown_0813607C
 * are `-fforce-addr` .rodata words holding &gUnknown_03001FBC and
 * &gUnknown_020296B0 (dumped from baserom.gba).
 *
 * The sound lookup uses the STRUCT MEMBER view (gUnknown_085D6C88[..].unk0c),
 * not the gUnknown_085D6C94 row symbol SubmarineFigure_Loop uses for the same table --
 * the ROM keeps the `+0xc` as a run-time `adds rB,#0xc` here and folds it into
 * the relocation there, which is the distinction the two symbols exist for. */
void BattleshipFigure_Loop(void)
{
    u16 side;
    u16 slot;
    u16 t;

    side = gUnknown_03001470[gUnknown_03001FBC].unk30;
    slot = gUnknown_03001470[gUnknown_03001FBC].unk34;
    t = sub_080156C4(gUnknown_03001FBC);

    if (t == 2)
    {
        if (gUnknown_020296B0[side].unk0c[gUnknown_020296B0[side].unk18] != 0xff)
        {
            SpawnThirdEffectAndProjectile(side, gUnknown_02029A10[side].entries[slot].unk1e);
            gUnknown_02029A10[side].entries[slot].unk1e++;
            PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[side][1]]
                             .unk0c[gUnknown_03004580[side][2] - 1]
                                   [gUnknown_020296B0[side].unk1a & 1]);
            gUnknown_020296B0[side].unk1a++;
        }
        else if (gUnknown_02029A10[side].entries[slot].unk18 != -1
                 && IsSlotScriptActiveAt(gUnknown_02029A10[side].entries[slot].unk18))
        {
            sub_080156E8(gUnknown_02029A10[side].entries[slot].unk18,
                         gUnknown_02029BA8[side].unk04);
        }
        gUnknown_020296B0[side].unk18++;
    }

    sub_0804CA98(side, slot, gUnknown_03001FBC);
    RidePartOnFigure(side, slot, gUnknown_03001FBC);
    StepFigureSlide(side, slot);
}
asm(".global sub_0804DF00\n.thumb_set sub_0804DF00, BattleshipFigure_Loop\n");
