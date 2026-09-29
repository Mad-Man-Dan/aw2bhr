#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B83C.
 * sub_0803B83C @ 0x0803B83C, sub_0803B858 @ 0x0803B858
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Two statements: stop the gUnknown_0849B048 blob through sub_0801537C, then
 * start the gUnknown_0849E7F8 proc on tree 3. sub_0801537C returns `int` and
 * the result is dropped -- the value is never re-narrowed and `pop {r0}` makes
 * this void, so the two `bl`s are sequential statements and not a nesting.
 * Proc_Start's result is discarded too. */

void ReturnToMainMenu(void)
{
    sub_0801537C(gUnknown_0849B048);
    Proc_Start(gUnknown_0849E7F8, PROC_TREE_3);
}

/* Three statements, all results discarded: BackupCampaignFlags, then sub_08085AF4,
 * then start the ProcScr_Link proc on tree 3. The first two are both
 * declared void(void) already and neither reads r0, so the run of `bl`s is
 * three statements rather than any nesting.
 * `pop {r0}; bx r0` -> void. */

void sub_0803B858(void)
{
    BackupCampaignFlags();
    sub_08085AF4();
    Proc_Start(ProcScr_Link, PROC_TREE_3);
}

asm(".global sub_0803B83C\n.thumb_set sub_0803B83C, ReturnToMainMenu\n");

extern void ResetRulesAfterCampaignMap(void);

struct ProcCmd CONST_DATA ProcScr_Link[] =
{
    PROC_1D(30),
    PROC_CALL(ResetRulesAfterCampaignMap),
    PROC_GOTO_SCR((void *)0x0849ECA4),
};

asm(".global gUnknown_0849EC8C\n.set gUnknown_0849EC8C, ProcScr_Link\n");
