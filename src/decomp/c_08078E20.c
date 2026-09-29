#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078E20.
 * sub_08078E20 @ 0x08078E20, sub_08078E48 @ 0x08078E48, sub_08078E94 @ 0x08078E94
 */

#include "hardware.h"
#include "proc.h"

/* A membership test over six sparse values. The `movs r0, #1` / `movs r0, #0`
 * split across an unconditional `b` is the one spelling `return <cmp>;` cannot
 * produce (docs/agbcc-codegen.md), so this is an explicit if/else and not a
 * returned comparison.
 *
 * RETURN TYPE CORRECTED to u8 in the second pass of wave 24, from the caller:
 * ResultsScreen_StartMusic re-narrows the result with a BARE `lsls r0, #0x18` before
 * `cmp r0, #0`. The body is byte-identical either way -- 0 and 1 need no
 * narrowing on the way out -- so nothing here could have settled it. */

u8 IsCampaignMilestoneMission(void)
{
    s32 x = GetCampaignMissionId();

    if (x == 8 || x == 0x10 || x == 0x18 || x == 0x20 || x == 0x22 || x == 0x2a)
        return 1;

    return 0;
}
asm(".global sub_08078E20\n.thumb_set sub_08078E20, IsCampaignMilestoneMission\n");

/* `movs r0, #3; rsbs r0, r0, #0; ands` is -3 == ~2, so this clears ONE bit --
 * bit 1 of gDispIo's byte 1, i.e. DISPCNT bit 9, bg1_enable. It is not `& ~3`.
 * The negation form rather than a direct `movs #0xfd` is the bitfield tell
 * described on gUnknown_03000564 in include/unknown-globals.h: the mask is
 * built in SImode, where ~2 is not an imm8. BootIntoWorldMap's `movs r0, #0x7f`
 * for forced_blank is the same construct with a mask that happens to narrow.
 *
 * GetResultsArmy's result is re-narrowed `lsls #0x10; lsrs #0x10` and consumed
 * by LoadBg1WindowFrame(u16), so it is a nest and the callee returns u16. */

/* WAVE 53, W53-D: the ProcPtr parameter is forced by SetupMenuScreenBgs gaining one
 * (see include/unknown-functions.h) and costs nothing -- r0 already holds it at
 * the call. Nothing in C calls ResultsScreen_Init; it is reached as a proc-script
 * function pointer, so no caller needed updating. Re-verified byte-for-byte. */
void ResultsScreen_Init(ProcPtr proc)
{
    SetupMenuScreenBgs(proc);
    SyncAllBgTilemaps();
    gDispIo.disp_ct.bg1_enable = 0;
    BG_EnableSyncBG2();
    LoadBg1WindowFrame(GetResultsArmy());
    LoadBg3MapBackdrop(gUnknown_085C77A0[gPlaySt.mapID].unk16, 0);
}
asm(".global sub_08078E48\n.thumb_set sub_08078E48, ResultsScreen_Init\n");

/* `movs r0, #0xd1; lsls r0, r0, #1` is a plain 0x1a2 -- minimal shift, one
 * register -- not wave 23's named constant local.
 *
 * The bare `lsls r0, #0x18` on IsCampaignMilestoneMission's result is what retyped that
 * function's return from s32 to u8; see the note on its prototype in
 * include/unknown-functions.h. */

void ResultsScreen_StartMusic(ProcPtr parent)
{
    SetSoundMixerChannelCount(9);

    if (IsCampaignMilestoneMission())
        PlayMusic(0x1a2);
    else
        Proc_Start(gUnknown_08615D70, parent);
}
asm(".global sub_08078E94\n.thumb_set sub_08078E94, ResultsScreen_StartMusic\n");
