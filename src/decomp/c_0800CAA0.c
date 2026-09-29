#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800CAA0.
 * sub_0800CAA0 @ 0x0800CAA0
 */

struct Unk88Rows
{
    /* 0x00 */ u8 row0[0x10];
    /* 0x10 */ u8 row1[0x10];
};
#define ROW1(sym) (((struct Unk88Rows *)(sym))->row1)

/* Returns the index (0-15) of the first palette-set entry whose four
 * colour codes (row 1 of each of the four 0x084888A0-block tables, >> 1)
 * equal gPlaySt.armyColor[1..4]; 0 if none match. sub_0800CB30 reads
 * the chosen entry back the same way.
 *
 * All four tables must be read as ROW1(sym)[i] (the member reference
 * sub_0800CB30 uses). Mixing in the row-1 symbols gUnknown_084888B0/D0/F0
 * changes which bases loop.c hoists: the ROM re-materialises only the
 * fourth base inside the loop, and every mixed spelling holds it
 * (+8 to +12 bytes). */
int GetArmyColorSetIndex(void)
{
    int i;
    int result = 0;

    for (i = 0; i < 16; i++)
    {
        if ((ROW1(gUnknown_084888A0)[i] >> 1) == gPlaySt.armyColor[1]
            && (ROW1(gUnknown_084888C0)[i] >> 1) == gPlaySt.armyColor[2]
            && (ROW1(gUnknown_084888E0)[i] >> 1) == gPlaySt.armyColor[3]
            && (ROW1(gUnknown_08488900)[i] >> 1) == gPlaySt.armyColor[4])
        {
            result = i;
            break;
        }
    }
    return result;
}
asm(".global sub_0800CAA0\n.thumb_set sub_0800CAA0, GetArmyColorSetIndex\n");
