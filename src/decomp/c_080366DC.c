#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080366DC.
 * sub_080366DC @ 0x080366DC
 */

void (*GetMainLoopCallback(void))(void)
{
    return gUnknown_030040EC;
}

asm(".global sub_080366DC\n.thumb_set sub_080366DC, GetMainLoopCallback\n");
