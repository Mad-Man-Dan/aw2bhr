#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061CF8.
 * sub_08061CF8 @ 0x08061CF8
 */

/* Four independent `if`s over the four low bits of one flag byte, each adding
 * GetArmyFacilityCount(n) into the accumulator. The element expression is repeated
 * rather than bound to a local: a local would park the element address in a
 * callee-saved register, while the ROM recomputes `gUnknown_030033EC * 0x3c`
 * and the `adds r0,#0x2c` (past ldrb's imm5) inside every arm. */
void AiCountEnemyFacilities(void)
{
    gUnknown_03004788 = 0;

    if (gPlayers[gUnknown_030033EC].unk2c & 1)
        gUnknown_03004788 += GetArmyFacilityCount(1);

    if (gPlayers[gUnknown_030033EC].unk2c & 2)
        gUnknown_03004788 += GetArmyFacilityCount(2);

    if (gPlayers[gUnknown_030033EC].unk2c & 4)
        gUnknown_03004788 += GetArmyFacilityCount(3);

    if (gPlayers[gUnknown_030033EC].unk2c & 8)
        gUnknown_03004788 += GetArmyFacilityCount(4);
}
asm(".global sub_08061CF8\n.thumb_set sub_08061CF8, AiCountEnemyFacilities\n");
