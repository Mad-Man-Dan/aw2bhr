#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025D20.
 * sub_08025D20 @ 0x08025D20, sub_08025D40 @ 0x08025D40
 */

/* The `++` spelling is load-bearing. `int v = ...; if (v != 0xff) ... = v + 1;`
 * is the same 32 bytes but drops the `adds r0, r2, #0` copy: agbcc keeps the
 * incremented value in its own pseudo for a read-modify-write on a MEM, and
 * coalesces it away when the source names a local instead.
 */
void IncrementPlayerUnitsCreated(int index)
{
    if (gPlayers[index].unitCount != 0xff)
        gPlayers[index].unitCount++;
}
asm(".global sub_08025D20\n.thumb_set sub_08025D20, IncrementPlayerUnitsCreated\n");

void IncrementPlayerUnitsLost(int index)
{
    if (gPlayers[index].unitsLost != 0xff)
        gPlayers[index].unitsLost++;
}
asm(".global sub_08025D40\n.thumb_set sub_08025D40, IncrementPlayerUnitsLost\n");
