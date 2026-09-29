#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080485AC.
 * sub_080485AC @ 0x080485AC
 */

void StartUnitListScreen(void)
{
    sub_080152EC(gUnknown_084C21C8, 0);
}
asm(".global sub_080485AC\n.thumb_set sub_080485AC, StartUnitListScreen\n");
