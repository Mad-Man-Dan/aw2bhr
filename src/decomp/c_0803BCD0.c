#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BCD0.
 * sub_0803BCD0 @ 0x0803BCD0
 */

void sub_0803BCD0(u8 a)
{
    gPlaySt.mapID = a;
}
