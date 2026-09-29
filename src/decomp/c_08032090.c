#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032090.
 * sub_08032090 @ 0x08032090
 */

#include "proc.h"

void LinkGotoIfStandardMap(ProcPtr proc)
{
    if (gPlaySt.mapID < 0xb4)
        Proc_Goto(proc, 1);
}
asm(".global sub_08032090\n.thumb_set sub_08032090, LinkGotoIfStandardMap\n");
