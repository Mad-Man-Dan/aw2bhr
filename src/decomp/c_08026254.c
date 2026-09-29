#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026254.
 * sub_08026254 @ 0x08026254
 */

/* Picks a random entry out of the 0xff-terminated unlocked-CO list built in
 * gUnknown_020288A0. The count loop is a plain `while` that agbcc inverts, so
 * the pre-test reads element 0 with no index. The modulo is UNSIGNED
 * (__umodsi3): GetNextRandomNumber returns u32 and the u8 count promotes into it. */
u8 PickRandomUnlockedCo(void)
{
    u8 n;

    n = 0;
    while (gUnknown_020288A0[n] != 0xff)
        n++;

    return gUnknown_020288A0[GetNextRandomNumber() % n];
}
asm(".global sub_08026254\n.thumb_set sub_08026254, PickRandomUnlockedCo\n");
