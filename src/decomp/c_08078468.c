#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078468.
 * sub_08078468 @ 0x08078468
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

void WM_ConfirmExit_08078469(void)
{
    ApplyWindowFramePalette(0, 0);
    gUnknown_03002F08.unk00 = 0;
}

asm(".global sub_08078468\n.thumb_set sub_08078468, WM_ConfirmExit_08078469\n");

extern void WM_ConfirmExit_08078959(void);
extern bool8 IsAnyEventScriptRunning(void);
extern void ClearVBlankHooks(void);

struct ProcCmd CONST_DATA ProcScr_WM_ConfirmExit[] =
{
    PROC_CALL(WM_ConfirmExit_08078469),
    PROC_CALL(WM_ConfirmExit_08078959),
    PROC_WHILE(IsAnyEventScriptRunning),
    PROC_CALL(ClearVBlankHooks),
    PROC_END,
};

asm(".global gUnknown_08615BBC\n.set gUnknown_08615BBC, ProcScr_WM_ConfirmExit\n");
