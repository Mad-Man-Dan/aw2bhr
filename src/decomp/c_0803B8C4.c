#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B8C4.
 * sub_0803B8C4 @ 0x0803B8C4
 */

#include "proc.h"

void sub_0803B8C4(void)
{
    gPlaySt.gameMode = 3;
    Proc_Start(ProcScr_Versus, PROC_TREE_3);
}

extern void Versus_InitPlayState(void);
extern void Versus_InitArmyRoster(void);
extern u8 GetMainMenuLock(void);
extern void Versus_ResetCoPickIndex(void);
extern void BuildUnlockedCoCarousel(void);
extern void sub_0803BFA4(void);
extern void sub_08026290(void);
extern void StartMatchSetupScreen(void);
extern int sub_080670D8(void);
extern void ApplyMatchSettingsRecord(void);
extern void Versus_AssignRandomCos(void);
extern void EndBg3AutoScroll(void);
extern void sub_080364F4(void);

#define Versus_0803BDBD Versus_InitPlayState
#define Versus_0803BE61 Versus_InitArmyRoster
#define Versus_0803BEF9 Versus_ResetCoPickIndex
#define Versus_08043D5D BuildUnlockedCoCarousel
#define Versus_0803BFA5 sub_0803BFA4
#define Versus_08026291 sub_08026290
#define Versus_080670A1 StartMatchSetupScreen
#define Versus_WHILE_080670D9 sub_080670D8
#define Versus_0803C1D5 ApplyMatchSettingsRecord
#define Versus_0803BF99 Versus_AssignRandomCos
#define Versus_08037F71 EndBg3AutoScroll
#define Versus_080364F5 sub_080364F4

struct ProcCmd CONST_DATA ProcScr_Versus[] =
{
    PROC_1D(5),
    PROC_CALL(Versus_0803BDBD),
    PROC_GOTO_SCR((void *)0x0849ECF8),
    PROC_CALL(Versus_0803BE61),
    PROC_START_CHILD_BLOCKING((void *)0x08616C54),
    PROC_GOTO_IF_YES(GetMainMenuLock, 0),
    PROC_GOTO_SCR((void *)0x0849E7F8),
PROC_LABEL(0),
    PROC_GOTO_SCR((void *)0x0849ED30),
    PROC_END,
    PROC_CALL(Versus_0803BEF9),
    PROC_CALL(Versus_08043D5D),
    PROC_CALL(Versus_0803BFA5),
    PROC_GOTO_SCR((void *)0x0849ED50),
    PROC_CALL(Versus_08026291),
    PROC_CALL(Versus_080670A1),
    PROC_WHILE(Versus_WHILE_080670D9),
    PROC_GOTO_IF_YES(GetMainMenuLock, 0),
    PROC_GOTO_SCR((void *)0x0849ECF8),
PROC_LABEL(0),
    PROC_CALL(Versus_0803C1D5),
    PROC_GOTO_SCR((void *)0x0849ED90),
    PROC_CALL(Versus_0803BF99),
    PROC_CALL(Versus_08037F71),
    PROC_CALL(Versus_080364F5),
    PROC_END,
};

asm(".global gUnknown_0849ECE0\n.set gUnknown_0849ECE0, ProcScr_Versus\n");
