#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080443D8.
 * sub_080443D8 @ 0x080443D8
 */

#include "proc.h"

void StartCoPowerUnitEffects(ProcPtr parent)
{
    Proc_StartBlocking(gUnknown_084A0818, parent);
}
asm(".global sub_080443D8\n.thumb_set sub_080443D8, StartCoPowerUnitEffects\n");
