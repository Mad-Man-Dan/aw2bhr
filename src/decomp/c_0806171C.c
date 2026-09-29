#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806171C.
 * sub_0806171C @ 0x0806171C, sub_08061788 @ 0x08061788, AiBeginTurn @ 0x08061868
 */

/* The destination record is at 0x02029C54, which aw2bhr.lds does NOT name --
 * it falls inside gUnknown_02029C20's region at +0x34, and 0x02029C54 + 0x130
 * is gUnknown_02029D84 exactly, so the two 0x130-byte records are adjacent.
 * Reaching it through gUnknown_02029C20 is what keeps the split build linkable;
 * the address constant itself goes to this unit's .rodata, which is the ROM
 * word at 0x0816DB00 (currently named gUnknown_0816DB00 in data/data.s). */
#define SCRATCH ((struct Unk085771C4 *)(gUnknown_02029C20 + 0x34))

/* AiDriverStep @ 0x0806171C, 108 bytes.
 *
 * The switch subject is read `ldrh; lsls #0x10; asrs #0x10` -- an UNSIGNED
 * halfword load followed by a separate sign extension. No plain read of a
 * 16-bit object produces that: agbcc folds `sign_extend (mem:HI)` into
 * `movs rI,#0; ldrsh rD,[rB,rI]` whatever the declared signedness, and that
 * fold was probed directly on both an s16 global (gUnknown_03004780) and a u16
 * one (gUnknown_030033EC), through an intermediate s16 local, through an
 * intermediate u16 local, through `(s16)(u16)`, through `& 0xffff` and through
 * `*(u16 *)&`. All six give `ldrsh`. Only a load agbcc's combiner may not touch
 * -- a volatile-qualified read -- leaves the two halves apart.
 *
 * That is a REPRODUCTION, not a claim that the object is volatile:
 * src/decomp/c_08034394.c's RemoteTurn_ExecuteCommand reads the same symbol with the
 * register-offset `ldrsh` that only a non-volatile s16 object gives, and it is
 * matched. The two readings cannot both come from one declaration, so the
 * original's two translation units declared gUnknown_03004780 differently. The
 * header keeps the s16 that has the stronger evidence and this file spells its
 * own view locally.
 *
 * Case bodies come out in source label order, so the ROM's body order
 * 0,1,2,3,5,4 is the order the arms are written in; case 4 is last and is the
 * only one without a trailing `b`.
 */
void AiDriverStep(void)
{
    if (sub_08019260())
        return;

    switch ((s16)*(volatile u16 *)&gUnknown_03004780)
    {
    case 0:
        AiBeginTurn();
        break;
    case 1:
        AiStartNextPass();
        break;
    case 2:
        AiRunNextWorklistUnit();
        break;
    case 3:
        AiExecuteActionStep();
        break;
    case 5:
        AiProduceUnits();
        break;
    case 4:
        AiEndTurnStep();
        break;
    }
}
asm(".global sub_0806171C\n.thumb_set sub_0806171C, AiDriverStep\n");

/* Two arms that do NOT cross-jump: both end in the same CopyAiPersonality call but
 * they compare different values (unk27 reloaded off the element address r3 vs
 * the local v), which is the documented condition for the tails to stay
 * separate. Only the third call, after the join, is shared. */
void AiLoadPersonality(u16 a)
{
    u8 v;

    if (gUnknown_085C77A0[gPlaySt.mapID].unk27 != 0)
    {
        CopyAiPersonality(SCRATCH,
            &gUnknown_085771C4[gUnknown_0857690C[gUnknown_085C77A0[gPlaySt.mapID].unk27][gPlayers[a].co]]);
    }
    else
    {
        if (gUnknown_030046B8 <= 3)
            v = gUnknown_08576908[gUnknown_030046B8];
        else
            v = 4;

        CopyAiPersonality(SCRATCH,
            &gUnknown_085771C4[gUnknown_0857690C[v][gPlayers[a].co]]);
    }

    CopyAiPersonality(&gUnknown_02029D84, SCRATCH);
}
asm(".global sub_08061788\n.thumb_set sub_08061788, AiLoadPersonality\n");

/* The record selection is a TERNARY over one store, exactly as the matched
 * WorldMap_CommitMissionAndEndProcs (src/decomp/c_08077F30.c) spells the same idiom: each arm
 * computes only `index * 0x30` and a base biased by 0x24 or 0x28, and the two
 * fall into a shared `adds r0,r0,r2; ldr r0,[r0]; str r0,[r5]`.
 *
 * The two zero stores at the end are ONE chained assignment. The destination is
 * evaluated before the right-hand side, so `gUnknown_030044D8 = gUnknown_03004770 = 0`
 * emits both pool `ldr`s first (0x030044D8's, then 0x03004770's), then the
 * single `movs r0,#0`, then the `str` and the `strb` in that order. Two
 * separate statements interleave the pool loads with their stores instead. */
void AiBeginTurn(void)
{
    gFactoryUnitSchedule = IsHardCampaignMode()
        ? gUnknown_08615194[gPlaySt.mapID - 0x8a].factoryScriptHc
        : gUnknown_08615194[gPlaySt.mapID - 0x8a].factoryScriptNc;

    AiClearTerritoryCounters();
    AiCountEnemyFacilities();
    AiScanBuildableFacilities();
    AiLoadPersonality(gUnknown_030033EC);
    AiBuildCapturePlaneAndClearEscortTally();
    AiClearInfluenceGrid();
    AiUpdateInfluence(0);
    AiUpdateUnitModes();

    gUnknown_03004780 = 1;
    gUnknown_030044D8 = gUnknown_03004770 = 0;

    if (gPlayers[gUnknown_030033EC].teamColor == 5 && gFactoryUnitSchedule != 0)
        sub_080607E8();
}

asm(".global sub_08061868\n.thumb_set sub_08061868, AiBeginTurn\n");
