#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025D08.
 * sub_08025D08 @ 0x08025D08
 */

/* CORRECTION, wave 32 (W32-A): `int`, not `u8` -- see c_08025CF0.c, its twin.
 * GetTechScore takes this result with a bare `adds r1, r0, #0`. */
int GetPlayerUnitsLost(int index)
{
    return gPlayers[index].unitsLost;
}
asm(".global sub_08025D08\n.thumb_set sub_08025D08, GetPlayerUnitsLost\n");
