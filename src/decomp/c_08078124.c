#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078124.
 * sub_08078124 @ 0x08078124
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Family F011: `push {lr}; bl f; ldr r0,=g; movs r1,#N; bl h; pop {r0}; bx r0`.
 * Two statements and NOT a nest -- r0 is overwritten by the pool `ldr` between
 * the two calls, so nothing can survive from the first one. `pop {r0}` is the
 * void epilogue.
 *
 * FOUR OF THE FAMILY'S NINE MEMBERS HAVE Proc_Start AS THE SECOND CALLEE, so
 * F011 is not "a table handed to an installer" -- families.py clusters on
 * shape and erases the callee, and here that spans two unrelated subsystems.
 * `movs r1, #3` is PROC_TREE_3, i.e. a root proc: this wrapper has no
 * parameter of its own for r1 to be forwarded from, which is the case the
 * proc-starter table in docs/agbcc-codegen.md distinguishes by r1 being
 * WRITTEN rather than left alone. Proc_Start returns the proc; `pop {r0}` says
 * this discards it. */
void Campaign_08078125(void)
{
    ClearWorldMapMarkers();
    Proc_Start(gUnknown_086147FC, PROC_TREE_3);
}

asm(".global sub_08078124\n.thumb_set sub_08078124, Campaign_08078125\n");

extern int Campaign_WHILE_08078151(void);
extern u8 GetMainMenuLock(void);
extern void ResetRulesAfterCampaignMap(void);

struct ProcCmd CONST_DATA ProcScr_Campaign[] =
{
    PROC_1D(5),
    PROC_START_CHILD_BLOCKING(ProcScr_CampaignIntro),
    PROC_CALL(Campaign_08078125),
    PROC_WHILE(Campaign_WHILE_08078151),
    PROC_GOTO_IF_NO(GetMainMenuLock, 100),
    PROC_CALL(ResetRulesAfterCampaignMap),
    PROC_GOTO_SCR((void *)0x0849EBFC),
};

asm(".global gUnknown_0849EB34\n.set gUnknown_0849EB34, ProcScr_Campaign\n");
