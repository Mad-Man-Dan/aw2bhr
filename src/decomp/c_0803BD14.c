#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BD14.
 * sub_0803BD14 @ 0x0803BD14
 */

/* Returns int: the caller SetArmyCoIdsFromList keeps the result in a register and
 * compares it signed (`cmp r1, r3; bge` / `blt`) with no narrowing.
 *
 * `adds r2, #0x3c` on the base rather than a folded displacement is the
 * two-variable-terms case (element index and array index are both runtime),
 * not an address being taken. */
int sub_0803BD14(void)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        if (gUnknown_085C77A0[gPlaySt.mapID].unk3c[i] != 0xff)
            break;
    }

    return i;
}
