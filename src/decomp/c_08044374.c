#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044374.
 * sub_08044374 @ 0x08044374
 */

int GetCoPowerUseCount(int a1)
{
    return gPlayers[a1].unk25;
}
asm(".global sub_08044374\n.thumb_set sub_08044374, GetCoPowerUseCount\n");
