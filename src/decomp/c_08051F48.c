#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051F48.
 * sub_08051F48 @ 0x08051F48
 */

void SmokeEffect_Loop(void)
{
}
asm(".global sub_08051F48\n.thumb_set sub_08051F48, SmokeEffect_Loop\n");
