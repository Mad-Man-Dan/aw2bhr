#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807813C.
 * sub_0807813C @ 0x0807813C
 */

#include "proc.h"

void StartWorldMapForResume(void)
{
    Proc_Start(gUnknown_0861485C, PROC_TREE_3);
}
asm(".global sub_0807813C\n.thumb_set sub_0807813C, StartWorldMapForResume\n");
