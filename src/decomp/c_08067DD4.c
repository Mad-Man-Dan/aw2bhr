#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067DD4.
 * sub_08067DD4 @ 0x08067DD4
 */

#include "proc.h"

/* The parent arrives in r0 and is moved to r1 for the call, so it is this
 * function's only parameter; nothing is written to the new proc. */
void StartIntroBgBounceIn(ProcPtr parent)
{
    Proc_Start(gUnknown_085810E4, parent);
}
asm(".global sub_08067DD4\n.thumb_set sub_08067DD4, StartIntroBgBounceIn\n");
