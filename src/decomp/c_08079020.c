#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08079020.
 * sub_08079020 @ 0x08079020
 */

void ResultsScreen_Idle(void)
{
}
asm(".global sub_08079020\n.thumb_set sub_08079020, ResultsScreen_Idle\n");
