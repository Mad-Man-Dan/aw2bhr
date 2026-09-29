#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028874.
 * sub_08028874 @ 0x08028874
 */

/* `value` is `int`, RETYPED IN WAVE 30 (W30-B) from `u8`. Nothing in this body
 * can tell the two apart -- the value's only use is a `strb` into a u8 member,
 * which is byte-identical at every width -- so the evidence is entirely on the
 * caller side. DefeatArmy passes its own raw `int` parameter here with no
 * narrowing, where a `u8` parameter makes agbcc emit `lsls #0x18; lsrs #0x18`
 * in front of the `bl`. */
void RecordArmyDefeat(int index, int value)
{
    gPlayers[index].defeated = gUnknown_03004080;
    gPlayers[index].unk13 = value;
}
asm(".global sub_08028874\n.thumb_set sub_08028874, RecordArmyDefeat\n");
