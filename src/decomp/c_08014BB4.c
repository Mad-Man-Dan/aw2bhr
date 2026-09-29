#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014BB4.
 * sub_08014BB4 @ 0x08014BB4, sub_08014BC0 @ 0x08014BC0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

void DialogueBlock_CB_08014BB5(void)
{
    gUnknown_03002514 = 0;
}

void StartDialogueBlock(ProcPtr parent)
{
    Proc_Start(ProcScr_DialogueOnEnd, parent);
}
asm(".global sub_08014BC0\n.thumb_set sub_08014BC0, StartDialogueBlock\n");

asm(".global sub_08014BB4\n.thumb_set sub_08014BB4, DialogueBlock_CB_08014BB5\n");

struct ProcCmd CONST_DATA ProcScr_DialogueOnEnd[] =
{
    PROC_ONEND(DialogueBlock_CB_08014BB5),
    PROC_BLOCK,
};

asm(".global gUnknown_0848A140\n.set gUnknown_0848A140, ProcScr_DialogueOnEnd\n");
