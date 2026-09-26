#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045984.
 * sub_08045984 @ 0x08045984
 */

int sub_08045984(void)
{
    if (gPlayers[1].unk13 == 8)
        return 0;

    return 1;
}
