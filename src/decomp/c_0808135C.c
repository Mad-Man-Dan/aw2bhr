#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808135C.
 * sub_0808135C @ 0x0808135C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
struct Unk808135C
{
    /* 00 */ STRUCT_PAD(0x00, 0x4a);
    /* 4a */ s16 unk4a;
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ int unk60;
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
    /* 68 */ s16 unk68;
    /* 6a */ s16 unk6a;
};
void sub_08084600(struct Unk808135C *);

void MainMenuCarouselWheel_Init(struct Unk808135C *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    do
    {
        gUnknown_03002020 = 0;
        gUnknown_03002B28 = 0x10;
        gUnknown_03001FFC = gUnknown_03001FFC;

        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 4;
        gUnknown_030030E0.bits.target1_enable_bd = 0;
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x1800;
        gUnknown_030030E0.bits.target2_enable_bd = 0;
    } while (0);

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 3;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 2;

    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win0_enable_blend = 0;

    SetDispEnable(1, 1, 1, 1, 1);

    sub_0801F114();
    InitTilePool(2, (void *)0x06010000, 0x300, 0x19);
    LoadTilePoolGraphic(0x50);

    proc->unk4c = 0;
    proc->unk52 = gUnknown_03005934;
    proc->unk4e = 0;

    if (gUnknown_030058FC != 0)
    {
        StartHeaderBanner(gUnknown_085802D8, gUnknown_0200FC50, 0x140, 0, 1, 1, (int)proc);

        gUnknown_03002020 = 4;
        proc->unk4c = 0x20;

        if (gUnknown_0861696C[DivRem(proc->unk52 + 2, 6)] == 2
            || gUnknown_0861696C[DivRem(proc->unk52 + 2, 6)] == 4)
            proc->unk64 = 0;
        else
            proc->unk64 = 1;
    }
    else
    {
        StartHeaderBanner(gUnknown_085802D8, gUnknown_0200FC50, 0x140, 0, 1, 0, (int)proc);

        proc->unk64 = 0;
        gUnknown_030058FC = 1;
    }

    proc->unk66 = gUnknown_03005924;
    proc->unk68 = 0;
    proc->unk6a = 0;
    proc->unk58 = 0;
    proc->unk5c = 0;
    proc->unk60 = 0;
    proc->unk4a = 0;

    gUnknown_0300591C[0] = proc->unk52;
    gUnknown_0300591C[1] = proc->unk64;
    gUnknown_0300591C[2] = proc->unk66;

    sub_08084600(proc);

    if (proc->unk66 == 6)
    {
        switch (gUnknown_0861696C[DivRem(proc->unk52 + 2, 6)])
        {
        case 0:
            if (GetCampaignSaveFlag() == 0)
                proc->unk66 = 7;

            break;

        case 1:
            if (GetVersusSaveFlag() == 0)
                proc->unk66 = 7;

            break;

        case 5:
            if (GetWarRoomSaveFlag() == 0)
                proc->unk66 = 7;

            break;
        }
    }
}

asm(".global sub_0808135C\n.thumb_set sub_0808135C, MainMenuCarouselWheel_Init\n");

extern void MainMenuCarouselWheel_TilesSlideInLoop(void);
extern void MainMenuCarouselWheel_CentreTilePopLoop(void);
extern void MainMenuCarouselWheel_LabelSlideInLoop(void);
extern void MainMenuCarouselWheel_InputLoop(void);

struct ProcCmd CONST_DATA ProcScr_MainMenuC2[] =
{
    PROC_CALL(MainMenuCarouselWheel_Init),
    PROC_SLEEP(7),
    PROC_REPEAT(MainMenuCarouselWheel_TilesSlideInLoop),
    PROC_REPEAT(MainMenuCarouselWheel_CentreTilePopLoop),
    PROC_REPEAT(MainMenuCarouselWheel_LabelSlideInLoop),
    PROC_REPEAT(MainMenuCarouselWheel_InputLoop),
    PROC_END,
};

asm(".global gUnknown_08616A08\n.set gUnknown_08616A08, ProcScr_MainMenuC2\n");
