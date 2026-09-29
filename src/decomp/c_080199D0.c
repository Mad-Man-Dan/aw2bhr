#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080199D0.
 * sub_080199D0 @ 0x080199D0
 */

void SetChoiceResult(u8 a)
{
    gUnknown_03002EE4 = a;
}
asm(".global sub_080199D0\n.thumb_set sub_080199D0, SetChoiceResult\n");
