#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080362D8.
 * sub_080362D8 @ 0x080362D8, sub_080362DC @ 0x080362DC
 */

void MoveSlideState_Nop0(void)
{
}
asm(".global sub_080362D8\n.thumb_set sub_080362D8, MoveSlideState_Nop0\n");

void MoveSlideState_Nop1(void)
{
}
asm(".global sub_080362DC\n.thumb_set sub_080362DC, MoveSlideState_Nop1\n");
