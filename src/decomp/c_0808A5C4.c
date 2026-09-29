#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A5C4.
 * sub_0808A5C4 @ 0x0808A5C4
 */

/* The first arm's sixth argument is `0` spelled as the value just compared:
 * the ROM stores the compare register straight into [sp,#4] where the second
 * arm has to materialise a fresh `movs r0,#0`. That is agbcc reusing a known
 * zero, not two different arguments -- writing 0 in both arms reproduces both. */
void CoDesignEditor_DrawHelpText(void)
{
    sub_08013C00();
    sub_0801B780(0);
    ApplyPaletteExt(gUnknown_081320AC, 0x60, 0x20);

    if (gUnknown_03005908 == 0)
        sub_08014A5C(0, 0x12, gBG0TilemapBuffer, 0x973, 0x3000, 0);
    else if (gUnknown_03005908 == 1)
        sub_08014A5C(0, 0x12, gBG0TilemapBuffer, 0x974, 0x3000, 0);

    sub_08013AEC();
}
asm(".global sub_0808A5C4\n.thumb_set sub_0808A5C4, CoDesignEditor_DrawHelpText\n");
