#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080386DC.
 * sub_080386DC @ 0x080386DC
 */

void sub_080386DC(int a, int b)
{
    gUnknown_0849D5F8->unk1e = a;
    gUnknown_0849D5F8->unk1f = b;
}
