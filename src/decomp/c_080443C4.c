#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080443C4.
 * sub_080443C4 @ 0x080443C4
 */

#include "proc.h"

void StartCoPowerWhiteFlash(ProcPtr parent)
{
    Proc_StartBlocking(gUnknown_084A07E8, parent);
}
asm(".global sub_080443C4\n.thumb_set sub_080443C4, StartCoPowerWhiteFlash\n");
