#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077A14.
 * sub_08077A14 @ 0x08077A14
 */

#include "proc.h"
#include "hardware.h"
/* The 0x5c-stride copy loop is the whole difficulty here. It reads out as a
 * NESTED array reference, `gUnknown_085C77A0[proc->unk34].unk40[i]`, and no
 * flat spelling reaches it: `((u8 *)gUnknown_085C77DC)[i + proc->unk34 * 0x5c
 * + 4]` (gUnknown_085C77DC is the same table 0x3c in), the same with the sum
 * re-associated, `(&gUnknown_085C77DC[x].unk01[3])[i]` and
 * `((u8 *)&gUnknown_085C77DC[x])[i + 4]` were all measured by compile_probe and
 * all four emit the 0x5c constant BEFORE the base's pool `ldr` in the preheader
 * and `adds rD, rPROD, rI` with the product as the first operand. The two-level
 * form is the only one that hoists the base first, puts the 0x5c
 * materialisation AFTER the giv init, and makes the loop counter the first
 * operand of the add. The +0x40 folds into the pool word's addend, which is the
 * symbol the splitter calls gUnknown_085C77E0.
 *
 * The destination is gPlayers[i + 1] -- slots 1..4, the army slots --
 * and fold turns (i + 1) * 0x3c + 0x1a into the ROM's i * 0x3c + 0x56.
 *
 * gUnknown_081CC5EC is not an object: it is the -fforce-addr word holding
 * &gUnknown_03002B6C, so this match needs "rodata": ["0x081CC5EC"] in
 * data/promoted.json. */

struct Unk8077A14Proc
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x0c];
    /* 0x44 */ int unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4a */ s16 unk4a;
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
};

void WorldMapMapPreview_Init(struct Unk8077A14Proc * proc)
{
    int i;
    int y;

    WorldMapMissionInfo_PutSprites(0, 0xa8 - gUnknown_0300064C);
    SetDefaultColorEffects();
    EndWorldMapSelectionFrame();
    ClearBg1Tilemap();
    SetBgScrollShadow(1, 0, 0);
    ClearAllUnits();
    CpuCopyAuto(gUnknown_080A0F38,
                 (void *)(0x06001000 + (gUnknown_03002B6C.bits.chr_block << 14)),
                 0x200);
    ApplyPaletteExt(gUnknown_080A1138, 0x80, 0x40);
    StartMapPreviewPalette(4);

    for (i = 0; i < 4; i++)
        gPlayers[i + 1].teamColor = gUnknown_085C77A0[proc->unk34].unk40[i];

    SnapshotTeamColorsFromPlayers();
    sub_0801B6EC((void *)(0x06001000 + (gUnknown_03002B6C.bits.chr_block << 14)));
    sub_0801B6FC((void *)(0x06001000 + (gUnknown_03002B6C.bits.chr_block << 14)));
    FillMapPreviewTilemap(gUnknown_08551A04 + 0x100, 0x4080);

    proc->unk4c = ((u8 *)gUnknown_03003F68)[0];
    proc->unk4e = ((u8 *)gUnknown_03003F68)[1];

    if (proc->unk4c & 1)
        proc->unk4c++;

    if (proc->unk4e & 1)
        proc->unk4e++;

    proc->unk4c = proc->unk4c >> 1;
    proc->unk4e = proc->unk4e >> 1;

    proc->unk48 = (0x1f - proc->unk4c) >> 1;

    y = (0x12 - proc->unk4e) >> 1;
    proc->unk4a = y;

    if (y < 0)
        proc->unk4a = 0;

    proc->unk44 = 0;
    SetDifficultyStarsPalette(3);
    SetWorldMapScopePalette(3);
}
asm(".global sub_08077A14\n.thumb_set sub_08077A14, WorldMapMapPreview_Init\n");
