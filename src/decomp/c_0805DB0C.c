#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DB0C.
 * sub_0805DB0C @ 0x0805DB0C
 */

/* Ends the battle animation once the defender's HP has run past the tracked
 * value, or once IsCoPowerActive says so for the current army.
 *
 * `% 100` comes out as __umodsi3 and not __modsi3 even though the u8 promotes
 * to a signed int: agbcc knows a zero-extended byte is non-negative and picks
 * the unsigned helper. The `lsls #0x18; lsrs #0x18` after it is the (u8)
 * conversion for the comparison against the u8 at *gUnknown_03004784.
 *
 * unk07[3] is offset 0x0a. The member array is left as it is rather than split
 * into named bytes -- include/unknown-globals.h records that its extent 5 is
 * corroborated by struct Unit's size, and a constant index folds into
 * the same `ldrb [rB, #0xa]` a named member would give. */
void AiDeliberateIndirectFire(void)
{
    AiStayHomeIfEnemyInfantryNear();

    if (*gUnknown_03004784 > (u8)(gUnknown_030040D8->unk07[3] % 100)
        || IsCoPowerActive(gUnknown_030033EC))
        AiTryAttack();
}
asm(".global sub_0805DB0C\n.thumb_set sub_0805DB0C, AiDeliberateIndirectFire\n");
