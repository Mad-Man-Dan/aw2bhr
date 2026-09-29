#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C76C.
 * sub_0806C76C @ 0x0806C76C
 */

#include "hardware.h"
#include "proc.h"

void WaitForAOrStart(ProcPtr proc)
{
    if (gpKeySt->pressed & 9)
        Proc_Break(proc);
}
asm(".global sub_0806C76C\n.thumb_set sub_0806C76C, WaitForAOrStart\n");
