#include "global.h"
#include "proc.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080191B0.
 * sub_080191B0 @ 0x080191B0
 */

/*
 * ResetEventScriptsAndUiState -- reset the gUnknown_0200C528 script system.
 *
 * Clears all ten slots and the eight gUnknown_0200C508 script pointers,
 * re-initialises the subsystems that hang off them, sets gUnknown_03002F08's
 * palette and CO words to 8 and 0xFFFF, and re-uploads the two 0x200-byte tile
 * blocks to BG VRAM. It is the first command of ProcScr_MainMenu below.
 *
 * The name comes from src/aw2e-names.s, where the proc-table labels are
 * generated from AW2E.lua. The `.thumb_set` below keeps sub_080191B0 working as
 * an alias, so other units resolve it unchanged.
 *
 * Why the C looks odd: both loops count an `s16`, which the compiler keeps
 * zero-extended and sign-extends at each use, so the index arithmetic is
 * recomputed every pass rather than being reduced to a running pointer. An int
 * counter tidies that up and the output no longer matches.
 */
void ResetEventScriptsAndUiState(void)
{
    s16 i;

    sub_080198C4();

    for (i = 0; i < 10; i++)
        gUnknown_0200C528[i].unk00 = NULL;

    for (i = 0; i < 8; i++)
        gUnknown_0200C508[i] = NULL;

    ClearCampaignFlagBank0();
    gUnknown_03002EF0 = 0;
    gUnknown_03001404 = 0;
    ResumeEventScripts();
    sub_080179AC();
    FillBlankBgTilemapAndSetScroll();
    gUnknown_03002F08.unk00 = 8;
    gUnknown_03002F08.unk02 = 0xFFFF;
    CpuCopyAuto(gUnknown_08499588, (void *)0x06006800, 0x200);
    CpuCopyAuto(gUnknown_0849958C, (void *)0x0600E000, 0x200);
}

asm(".global sub_080191B0\n.thumb_set sub_080191B0, ResetEventScriptsAndUiState\n");

extern void ResetMapSelectState(void);
extern void MainMenuCarousel_ResetSelection(void);
extern void MainMenu_0803BBD5(void);
extern void ClearSavingEnabled(void);
extern void RefreshDesignRoomSlotDirectory(void);
extern u8 GetMainMenuLock(void);
extern void MainMenu2_0803BBA9(void);
extern void StartIntroSequence(void);

struct ProcCmd CONST_DATA ProcScr_MainMenu[] =
{
    PROC_CALL(ResetEventScriptsAndUiState),
    PROC_CALL(ResetMapSelectState),
    PROC_CALL(MainMenuCarousel_ResetSelection),
    PROC_CALL(MainMenu_0803BBD5),
    PROC_GOTO_SCR(ProcScr_MainMenu2),
};

struct ProcCmd CONST_DATA ProcScr_MainMenu2[] =
{
    PROC_CALL(ClearSavingEnabled),
    PROC_CALL(RefreshDesignRoomSlotDirectory),
    PROC_START_CHILD_BLOCKING(ProcScr_MainMenuC1),
    PROC_GOTO_IF_NO(GetMainMenuLock, 0),
    PROC_1D(30),
    PROC_CALL(MainMenu2_0803BBA9),
    PROC_GOTO(1),
PROC_LABEL(0),
    PROC_CALL(StartIntroSequence),
PROC_LABEL(1),
    PROC_END,
};
