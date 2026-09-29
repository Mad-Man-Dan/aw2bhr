#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044080.
 * sub_08044080 @ 0x08044080
 */

void SetCoPowerCharge(int a1, u32 a2)
{
    gPlayers[a1].coCharge = a2;
}
asm(".global sub_08044080\n.thumb_set sub_08044080, SetCoPowerCharge\n");
