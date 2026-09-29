#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080198AC.
 * sub_080198AC @ 0x080198AC
 */

void ClearCoScreenDrawHook(void)
{
    gUnknown_03002F20 = 0;
}
asm(".global sub_080198AC\n.thumb_set sub_080198AC, ClearCoScreenDrawHook\n");
