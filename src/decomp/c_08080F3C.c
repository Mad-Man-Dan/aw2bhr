#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080F3C.
 * sub_08080F3C @ 0x08080F3C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void MainMenuCarousel_ResetSelection(void)
{
    gUnknown_03005924 = 6;
    gUnknown_030058FC = 0;
}

asm(".global sub_08080F3C\n.thumb_set sub_08080F3C, MainMenuCarousel_ResetSelection\n");
