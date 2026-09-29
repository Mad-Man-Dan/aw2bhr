#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807639C.
 * sub_0807639C @ 0x0807639C
 */

#include "proc.h"

void StartWorldMapNationPanel(ProcPtr parent)
{
    Proc_Start(ProcScr_WM_Listener, parent);
}
asm(".global sub_0807639C\n.thumb_set sub_0807639C, StartWorldMapNationPanel\n");
