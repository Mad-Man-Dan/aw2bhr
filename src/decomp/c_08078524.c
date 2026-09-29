#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078524.
 * sub_08078524 @ 0x08078524
 */

#include "proc.h"

/* Family F032, byte-identical to sub_08049928 and WorldMapScene_WaitScript. See the note
 * on sub_08049928 for why sub_08019260 is nullary and returns bool8. */

void BlockingEventScript_Wait(ProcPtr proc)
{
    if (sub_08019260() == 0)
        Proc_Break(proc);
}
asm(".global sub_08078524\n.thumb_set sub_08078524, BlockingEventScript_Wait\n");
