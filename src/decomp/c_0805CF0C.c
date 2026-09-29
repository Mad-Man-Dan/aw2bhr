#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805CF0C.
 * sub_0805CF0C @ 0x0805CF0C
 */

/* One of the thirteen list builders at 0x0805CA60-0x0805D1F0.  This is one of the three (family
 * F089, 176 bytes) that do NOT call sub_0805D344; the seven promoted
 * 188-byte siblings do.
 * Otherwise this is c_0805CA60.c exactly: the DIRECT spelling
 * `gUnknown_030046B0 = gUnknown_030045F0` by name, which is what the original
 * source said.  agbcc's -fforce-addr parks the two addresses in this unit's own
 * .rodata because both are live across the builder's loop; the two words are the
 * ROM's gUnknown_0816DA10 / gUnknown_0816DA14 and the build now places them there
 * (tools/split_rodata.py).  Those two ROM words hold 0x030046B0 and 0x030045F0,
 * the same values as every other slot in the 0x0816D9E0-0x0816DA37 block (one
 * pair per builder, address order), so they are NOT globals of their own.
 */

void AiStartFootUnitPass(void)
{{
    int i;

    gUnknown_030046B0 = gUnknown_030045F0;

    for (i = gUnknown_03003F2C + 1; i < gUnknown_03003F2C + 0x40; i++)
    {{
        if (gUnits[i].type != 0
            && !(gUnits[i].flags & 1)
            && gUnknown_085D5ABC[gUnits[i].type].unk1b == 1)
        {{
            *gUnknown_030046B0++ = i;
        }}
    }}

    *gUnknown_030046B0 = 0;
    gUnknown_030046B0 = gUnknown_030045F0;
    gUnknown_03004778 = AiDeliberateFootUnit;
    gUnknown_03004780 = 2;
}}
asm(".global sub_0805CF0C\n.thumb_set sub_0805CF0C, AiStartFootUnitPass\n");
