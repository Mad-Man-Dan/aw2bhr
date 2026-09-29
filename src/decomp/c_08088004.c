#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08088004.
 * sub_08088004 @ 0x08088004
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"

void CoDesignRoot_StartEditor(ProcPtr proc)
{
    gUnknown_03005908 = 0;
    CoDesignEditor_DrawHelpText();
    LoadBg1WindowFrame(1);
    ApplyWindowFramePalette(gUnknown_03005958[0], 8);
    CoDesignEditor_SetupBlend();
    Proc_Start(ProcScr_CoDesignC2, proc);
}

asm(".global sub_08088004\n.thumb_set sub_08088004, CoDesignRoot_StartEditor\n");
