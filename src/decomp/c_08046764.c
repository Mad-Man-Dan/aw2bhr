#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08046764.
 * sub_08046764 @ 0x08046764
 */

void StartIntelStatusScreen(void)
{
    sub_080152EC(gUnknown_084C1824, 0);
}
asm(".global sub_08046764\n.thumb_set sub_08046764, StartIntelStatusScreen\n");
