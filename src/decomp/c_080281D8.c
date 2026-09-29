#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080281D8.
 * sub_080281D8 @ 0x080281D8
 */

/* Starts the gUnknown_08499EE4 script and parks the second argument in the new
 * slot's +0x18, where ApplyUnitSpawnTableFromSlot picks it up. The first parameter is dead --
 * r0 is overwritten by the pool word before anything reads it. */
void sub_080281D8(u8 a1, u32 a2)
{
    sub_080152EC(gUnknown_08499EE4, 0)->unk18 = a2;
}
