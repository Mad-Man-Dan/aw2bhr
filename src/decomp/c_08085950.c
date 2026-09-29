#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085950.
 * sub_08085950 @ 0x08085950
 */

/* `lsls #0x15; lsrs #0x10` is a NET LEFT SHIFT OF FIVE with a u16 truncation,
 * not a mask plus a shift -- and it has to be spelled as a multiply: with the
 * (u16) cast, shorten_binary_op folds `x * 0x20` into the two instructions the
 * ROM has, where `x << 5` needs three. */
void LoadCoInfoUnitSheet(int a1, int a2)
{
    RegisterDataMove(GetUnitSheetGraphics(), (void *)0x06012000, (GetUnitSheetFrameTileCount() & 0x3FF) << 5);

    ApplyPaletteExt((u16 *)GetArmyTeamColorPalette(a2), (u16)((a1 + 0x10) * 0x20), 0x20);
}
asm(".global sub_08085950\n.thumb_set sub_08085950, LoadCoInfoUnitSheet\n");
