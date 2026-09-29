#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DA18.
 * sub_0802DA18 @ 0x0802DA18
 */

/* MATCHED in wave 74 (W74-C): giving each directional key block its own
 * block-scoped `int v` shortens the two independent live ranges and produces
 * the ROM's late r1/r2/r3/r4 allocation exactly.  Configured verdict is
 * 440/440 with only the two accepted relocation-equivalent pool words:
 * 0x08090C18 and 0x08090C1C.
 *
 * Previously parked at 440/440 bytes, 97.5% identical (wave 65, W65-N).
 * SIZE-EXACT.
 * 11 differing bytes remain, in two allocation groups plus benign pool relocs:
 *   - the two `.rodata` force-addr words (0x08090C18 -> &gUnknown_03001FBC,
 *     0x08090C1C -> &gpKeySt). Benign; carry them at promotion.
 *   - the second `unk20` block picks r2/r3 where the ROM picks r1/r2, and the
 *     tail picks `movs r4,#30` where the ROM picks `movs r3,#30`. Downstream of
 *     the same allocation.
 * FIXED: embedding the table binding in the first indexed read,
 * `(tbl = (struct Unk2023830 *)gUnknown_02023830)[...].unk01`, moves the pool
 * `ldr` before the index scale without changing size and improves 95.7 -> 97.5%.
 * Binding
 * `tbl = (struct Unk2023830 *)gUnknown_02023830;` as its own statement at the
 * top of the block, which is the obvious fix -- it emits the `ldr` in the right
 * place but costs 4 bytes elsewhere and scores worse. The embedded assignment
 * is load-bearing and should remain.
 * SETTLED, do not re-derive: the struct-cast index
 * `((struct Unk2023830 *)gUnknown_02023830)[i].unk01` is required -- it is the
 * only spelling that puts the +1 in the load displacement instead of an extra
 * `adds` or a relocation addend (see docs/agbcc-codegen.md). The `(u16)` casts
 * on unk20/unk1e are also required and measured. */
#include "hardware.h"
struct Unk2023830
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 filler_02[0x02];
};

void DeploymentScreen_Loop(void)
{
    struct Unk03001470 *ent;
    void *p;
    int t;
    struct Unk2023830 *tbl;

    SetInfoBoxMode(1);

    if (IsCoSpeechScriptRunning())
        return;

    ent = &gUnknown_03001470[gUnknown_03001FBC];

    if (gpKeySt->pressed & 1)
    {
        if ((tbl = (struct Unk2023830 *)gUnknown_02023830)[ent->unk20].unk01 == 2)
        {
            PlayMusicOrSfx2(0x68);
            return;
        }

        PlayMusicOrSfx2(0x65);

        if (FindFreeUnitSlot() == NULL)
        {
            ShowUnitLimitMessage();
            return;
        }

        p = BuyUnit(gUnknown_030040A4.unk00, gUnknown_030040A4.unk02,
                         ((struct Unk2023830 *)gUnknown_02023830)[ent->unk20].unk00);

        gUnknown_03003100.pos.unk00 = gUnknown_030040A4.unk00;
        gUnknown_03003100.pos.unk02 = gUnknown_030040A4.unk02;

        if (gPlaySt.savingEnabled != 0)
            SendActionCommand(1, ((struct Unk2023830 *)gUnknown_02023830)[ent->unk20].unk00,
                         0, 0);

        RunMapEventsForAction(0xe, p);
    }

    if (gpKeySt->pressed & 2)
        PlayMusicOrSfx2(0x66);

    if (gpKeySt->pressed & 3)
    {
        ClearSlotScriptCallback(gUnknown_03001FBC);
        ClearDeploymentListArea();
        EndUnitInfoPanelScripts();
        return;
    }

    if (gpKeySt->repeated & DPAD_DOWN)
    {
        int v;

        v = (u16)ent->unk20;

        if (ent->unk20 < ent->unk22)
        {
            ent->unk20 = v + 1;
            sub_0802D9B8(ent);
            PlayMusicOrSfx2(0x67);
        }
    }

    if (gpKeySt->repeated & DPAD_UP)
    {
        int v;

        v = (u16)ent->unk20;

        if (ent->unk20 > 0)
        {
            ent->unk20 = v - 1;
            sub_0802D9B8(ent);
            PlayMusicOrSfx2(0x67);
        }
    }

    gUnknown_030033E4.unk00 = 1;

    t = (u16)ent->unk20 - (u16)ent->unk1e + 2;
    gUnknown_030033E4.unk02 = t;
    EaseMapCursorAndDraw(0x10, t * 16 + 8, 3);

    if (ent->unk1e != 0)
        DrawMapCursorSprite(0x44, 0x2a, 0xe);

    if (ent->unk1e + 7 < *(s16 *)&gUnknown_0300055A)
        DrawMapCursorSprite(0x44, 0x96, 0xf);
}
asm(".global sub_0802DA18\n.thumb_set sub_0802DA18, DeploymentScreen_Loop\n");
