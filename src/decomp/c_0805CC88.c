#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805CC88.
 * sub_0805CC88 @ 0x0805CC88
 */

/* One of the thirteen list builders at 0x0805CA60-0x0805D1F0.  This one is the odd member: its
 * third filter is a RANGE on the unit id itself rather than a lookup in
 * gUnknown_085D5ABC, so it carries no pool word for that table and saves only
 * one high register.  `(u8)(unk00 - 0x10) <= 1` is the ROM's
 * `subs #0x10; lsls #0x18; lsrs #0x18; cmp #1; bhi` -- a u8 truncation of the
 * difference and an UNSIGNED compare, which is exactly how a two-value id
 * range is written without a second branch.
 * Otherwise this is c_0805CA60.c exactly: the DIRECT spelling
 * `gUnknown_030046B0 = gUnknown_030045F0` by name, which is what the original
 * source said.  agbcc's -fforce-addr parks the two addresses in this unit's own
 * .rodata because both are live across the builder's loop; the two words are the
 * ROM's gUnknown_0816D9F8 / gUnknown_0816D9FC and the build now places them there
 * (tools/split_rodata.py).  Those two ROM words hold 0x030046B0 and 0x030045F0,
 * the same values as every other slot in the 0x0816D9E0-0x0816DA37 block (one
 * pair per builder, address order), so they are NOT globals of their own.
 */

void AiStartFighterBomberAttackPass(void)
{{
    int i;

    gUnknown_030046B0 = gUnknown_030045F0;

    for (i = gUnknown_03003F2C + 1; i < gUnknown_03003F2C + 0x40; i++)
    {{
        if (gUnits[i].type != 0
            && !(gUnits[i].flags & 1)
            && (u8)(gUnits[i].type - 0x10) <= 1)
        {{
            *gUnknown_030046B0++ = i;
        }}
    }}

    *gUnknown_030046B0 = 0;
    gUnknown_030046B0 = gUnknown_030045F0;
    sub_0805D344(gUnknown_0300477C);
    gUnknown_03004778 = AiDeliberateDirectAttackOrRoleMove;
    gUnknown_03004780 = 2;
}}
asm(".global sub_0805CC88\n.thumb_set sub_0805CC88, AiStartFighterBomberAttackPass\n");
