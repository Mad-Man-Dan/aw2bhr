#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019E2C.
 * sub_08019E2C @ 0x08019E2C, sub_08019E30 @ 0x08019E30, sub_08019E34 @ 0x08019E34
 */

int Menu_UsabilityShown(void)
{
    return 0;
}
asm(".global sub_08019E2C\n.thumb_set sub_08019E2C, Menu_UsabilityShown\n");

int Menu_UsabilityGreyed(void)
{
    return 2;
}
asm(".global sub_08019E30\n.thumb_set sub_08019E30, Menu_UsabilityGreyed\n");

int Menu_UsabilityHidden(void)
{
    return 1;
}
asm(".global sub_08019E34\n.thumb_set sub_08019E34, Menu_UsabilityHidden\n");
