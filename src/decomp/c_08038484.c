#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038484.
 * sub_08038484 @ 0x08038484
 */

/* THE 0x10 TABLE IS AN ARRAY, AND THE SHARED STRUCT SAYS IT IS TWO SCALARS.
 * gUnknown_0200C420's +0x10 and +0x12 are declared `u16 unk10; u16 unk12;` in
 * unknown-globals.h, but this function subscripts that pair with the runtime
 * value IsHardCampaignMode() returns. The difference is visible in the pool word and
 * is worth 3 instructions: `&gUnknown_0200C420.unk10` const-folds to the single
 * address constant `gUnknown_0200C420+0x10`, while the ROM's word is a BARE
 * `gUnknown_0200C420` with `adds r5,r4,#0; adds r5,#0x10` computing the member
 * offset at runtime -- which is what an array member emits, because the
 * variable subscript stops the fold.
 *   Reshaping unk10/unk12 into `u16 unk10[2]` is the reading the codegen
 * actually supports, but it is a SHARED member (ShopAvail_CoNeedsRank reads +0x10 twice,
 * ResetProfileToDefaults zeroes both) and the brief forbids reshaping one to suit a
 * single function, so the layout is spelled locally instead -- the same device
 * src/decomp/c_08038848.c uses for gUnknown_08499590. Recorded rather than
 * changed; if a second function turns up subscripting the same pair, that is
 * the evidence to promote it to an array in the header.
 *
 * `ok` is `int`: the `lsls #0x18; lsrs #0x18` after MissionTriggersUnlockScript is agbcc
 * re-narrowing that function's bool8 return, not a narrow local. The else arm
 * stores `ok` rather than a literal 0 -- the ROM reuses r4 there instead of
 * emitting a fresh `movs r0,#0`, which is what naming the variable produces. */
struct Unk38484Tbl
{
    /* 0x00 */ u8 filler_00[0x10];
    /* 0x10 */ u16 unk10[2];
};

void EndOfGame_FinishCampaignMap(void)
{
    struct Unk38484Tbl *tbl;
    int ok;

    ok = 0;

    if (IsPlayer1TeamAlive())
    {
        SaveCampaignMissionResult(gPlaySt.mapID - 0x8a, gUnknown_03004080,
                     gPlayers[GetResultsArmy()].totalScore);
        CampaignMapNoOp(gPlaySt.mapID - 0x8a);
        gUnknown_0202FDFC.unk0c = gPlaySt.mapID - 0x8a;
        gUnknown_0202FDFC.unk11 = 1;
        ok = MissionTriggersUnlockScript(gPlaySt.mapID - 0x8a);
    }
    else
    {
        gUnknown_0202FDFC.unk0c = gPlaySt.mapID - 0x8a;
        gUnknown_0202FDFC.unk11 = ok;
    }

    ResetRulesAfterCampaignMap();

    if (ok)
    {
        tbl = (struct Unk38484Tbl *)&gUnknown_0200C420;

        if (tbl->unk10[IsHardCampaignMode()] < GetAverageCampaignScore())
            tbl->unk10[IsHardCampaignMode()] = GetAverageCampaignScore();

        sub_08045790();
    }
    else
    {
        StartCampaignAfterMap();
    }
}
asm(".global sub_08038484\n.thumb_set sub_08038484, EndOfGame_FinishCampaignMap\n");
