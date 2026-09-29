#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805D134.
 * sub_0805D134 @ 0x0805D134
 */

/* One of the thirteen list builders at 0x0805CA60-0x0805D1F0.  This is the
 * DIRECT spelling -- `gUnknown_030046B0 = gUnknown_030045F0` by name, which is
 * what the original source said -- and it is every instruction and every
 * register of the original.  agbcc's -fforce-addr parks the two addresses in
 * this unit's own .rodata because both are live across the builder's loop; the
 * two words are the ROM's gUnknown_0816DA28 / gUnknown_0816DA2C and the build now
 * places them there (tools/split_rodata.py).  The `u8 **const` workaround that
 * this function was parked on for five waves is NOT used: it reached the right
 * instruction multiset but left the `adds r6, r3, #0` copy 4th in the loop
 * preheader where the ROM has it last, and no source spelling moved it.
 */
void AiStartIndirectRoleMovePass(void)
{
    int i;

    gUnknown_030046B0 = gUnknown_030045F0;

    for (i = gUnknown_03003F2C + 1; i < gUnknown_03003F2C + 0x40; i++)
    {
        if (gUnits[i].type != 0
            && !(gUnits[i].flags & 1)
            && gUnknown_085D5ABC[gUnits[i].type].unk1b == 4)
        {
            *gUnknown_030046B0++ = i;
        }
    }

    *gUnknown_030046B0 = 0;
    gUnknown_030046B0 = gUnknown_030045F0;
    sub_0805D344(gUnknown_0300477C);
    gUnknown_03004778 = AiDeliberateIndirectRoleMove;
    gUnknown_03004780 = 2;
}
asm(".global sub_0805D134\n.thumb_set sub_0805D134, AiStartIndirectRoleMovePass\n");
