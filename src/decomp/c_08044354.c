#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044354.
 * sub_08044354 @ 0x08044354
 */

void sub_08044354(int a1)
{
    if (gPlayers[a1].unk25 != 0xff)
        gPlayers[a1].unk25++;
}
