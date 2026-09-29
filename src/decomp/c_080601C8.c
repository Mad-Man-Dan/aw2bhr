#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080601C8.
 * sub_080601C8 @ 0x080601C8, sub_080601DC @ 0x080601DC
 */

void sub_080601C8(void)
{
    PayForCoPower(gUnknown_030046C0.unk06, 1);
}

void sub_080601DC(void)
{
    PayForCoPower(gUnknown_030046C0.unk06, 2);
}
