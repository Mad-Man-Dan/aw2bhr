#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B884.
 * sub_0807B884 @ 0x0807B884
 */

#include "proc.h"
#include "hardware.h"

/* VERIFIED MATCH (wave 54, W54-A) -- try_match reports matched, 440/440 bytes,
 * "relocs: match", byte-for-byte identical. No rodata placement needed: both
 * jump tables are ordinary in-unit .text tables and neither of this function's
 * globals goes through the 081D93xx pool.
 *
 * Two things worth keeping if this ever regresses:
 *
 * 1. THE CASE ORDER IS 1, 2, 4, 3, 5 -- not sorted. agbcc emits case bodies in
 *    SOURCE order and builds the jump table separately, so the table reads
 *    1,2,3,4,5 while the bodies sit at ascending addresses in the order
 *    1,2,4,3,5. Writing the cases sorted puts case 3's and case 4's bodies the
 *    wrong way round. Read the body order off the jump-table TARGETS, not off
 *    the table's index order.
 *
 * 2. Case 5 is `if (GetCampaignMissionId() == 0x22) song = 0x19f; else song = 0x198;`,
 *    with the CALL first. agbcc presets 0x198 and reaches 0x19f with
 *    `adds r4, #7`; writing `song = 0x198;` before the `if` instead puts the
 *    constant ahead of the `bl`, which the ROM does not.
 *
 * `song` is 0x198 on entry and every path that does not reassign it falls
 * through to PlayMusic(0x198) -- that initial assignment is a real source
 * statement placed BEFORE SetupMenuScreenBgs(proc), which is where the ROM's
 * `movs r4, #0xcc; lsls r4, r4, #1` sits.
 */

void MissionTitle_Setup(ProcPtr proc)
{
    int song;

    song = 0x198;

    SetupMenuScreenBgs(proc);

    Decompress(gUnknown_0822FEF0, (void *)((gUnknown_03001FE8.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_0822F9AC, gBG1TilemapBuffer);

    BG_EnableSyncBG1();

    ApplyPaletteExt(gUnknown_0822FE50, 0x20, 0xa0);

    Proc_Start(gUnknown_086165B0, proc);

    switch (gUnknown_085C77A0[gPlaySt.mapID].unk58)
    {
    case 1:
        song = 0x194;
        ApplyPaletteExt(gUnknown_0822DB88, 0, 0x20);
        break;

    case 2:
        song = 0x197;
        ApplyPaletteExt(gUnknown_0822DBA8, 0, 0x20);
        break;

    case 4:
        song = 0x196;
        ApplyPaletteExt(gUnknown_0822DBC8, 0, 0x20);
        break;

    case 3:
        song = 0x195;
        ApplyPaletteExt(gUnknown_0822DBE8, 0, 0x20);
        break;

    case 5:
        if (GetCampaignMissionId() == 0x22)
            song = 0x19f;
        else
            song = 0x198;

        ApplyPaletteExt(gUnknown_0822DB88, 0, 0x20);
        break;
    }

    switch (GetCampaignMissionId())
    {
    case 8:
    case 16:
    case 24:
    case 32:
    case 42:
        song = 0x198;
        break;
    }

    PlayMusic(song);
    Proc_Start(gUnknown_08616508, proc);
}
asm(".global sub_0807B884\n.thumb_set sub_0807B884, MissionTitle_Setup\n");
