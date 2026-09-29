#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080733B8.
 * sub_080733B8 @ 0x080733B8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

void EndHeaderBanner(void)
{
    Proc_EndEach(ProcScr_MainMenuPutSelectModeSprite);
}

asm(".global sub_080733B8\n.thumb_set sub_080733B8, EndHeaderBanner\n");
