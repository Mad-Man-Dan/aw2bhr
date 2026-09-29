#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025CF0.
 * sub_08025CF0 @ 0x08025CF0
 */

/* CORRECTION, wave 32 (W32-A): `int`, not `u8`. GetTechScore -- the first C
 * caller either of this pair has ever had -- takes the result with a bare
 * `adds r4, r0, #0` and no re-narrowing, and agbcc re-narrows a u8-returning
 * callee at every call site. Byte-identical here either way. */
int GetPlayerUnitsCreated(int index)
{
    return gPlayers[index].unitCount;
}
asm(".global sub_08025CF0\n.thumb_set sub_08025CF0, GetPlayerUnitsCreated\n");
