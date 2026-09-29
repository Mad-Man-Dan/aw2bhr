#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017970.
 * sub_08017970 @ 0x08017970
 */

void PauseEventScripts(void)
{
    gUnknown_03002B38 = 1;
}
asm(".global sub_08017970\n.thumb_set sub_08017970, PauseEventScripts\n");
