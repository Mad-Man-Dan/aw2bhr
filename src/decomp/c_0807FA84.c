#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807FA84.
 * sub_0807FA84 @ 0x0807FA84
 */

void CoPowerScene_Idle(void)
{
}
asm(".global sub_0807FA84\n.thumb_set sub_0807FA84, CoPowerScene_Idle\n");
