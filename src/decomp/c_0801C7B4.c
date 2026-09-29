#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C7B4.
 * sub_0801C7B4 @ 0x0801C7B4
 */

#include "proc.h"

/* `cmp; beq L; movs r0, #1; L:` with no second constant is the `return c;`
 * boolean form -- the zero is never materialised because the false path is the
 * fall-through of the call's own result. */
bool8 APProc_Exists(void)
{
    return Proc_Find(ProcScr_WaitForLaser) != NULL;
}
asm(".global sub_0801C7B4\n.thumb_set sub_0801C7B4, APProc_Exists\n");
