#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A638.
 * sub_0808A638 @ 0x0808A638, sub_0808A64C @ 0x0808A64C
 */

#include "proc.h"

void StartCoDesignEditor(void)
{
    Proc_Start(ProcScr_CoDesignC1, PROC_TREE_3);
}
asm(".global sub_0808A638\n.thumb_set sub_0808A638, StartCoDesignEditor\n");

int IsCoDesignEditorRunning(void)
{
    return Proc_Find(ProcScr_CoDesignC1) != 0;
}
asm(".global sub_0808A64C\n.thumb_set sub_0808A64C, IsCoDesignEditorRunning\n");
