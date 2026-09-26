#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044094.
 * GetCoPowerCharge @ 0x08044094
 */

u32 GetCoPowerCharge(int a1)
{
    return gPlayers[a1].coCharge;
}

asm(".global sub_08044094\n.thumb_set sub_08044094, GetCoPowerCharge\n");
