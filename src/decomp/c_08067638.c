#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067638.
 * sub_08067638 @ 0x08067638
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Loads the 0x08067 screen: three graphics blobs, one palette, and both BG
 * scrolls reset. `movs r1,#0xc0; lsls r1,#0x13` is the literal 0x06000000. */
void IntroT3_Child_08067639(void)
{
    Decompress(gUnknown_0817B970, (void *)0x06000000);
    Decompress(gUnknown_0817BE90, (void *)0x0600D800);
    Decompress(gUnknown_0817C138, (void *)0x0600E000);

    ApplyPaletteExt(gUnknown_0817C3E8, 0, 0x20);

    SetBgScrollShadow(0, 0, 0);
    SetBgScrollShadow(1, 0, 0);
}

asm(".global sub_08067638\n.thumb_set sub_08067638, IntroT3_Child_08067639\n");

extern void IntroT3_Child_IDLE_08067691(void);

struct ProcCmd CONST_DATA ProcScr_IntroT3Child[] =
{
    PROC_CALL(IntroT3_Child_08067639),
    PROC_REPEAT(IntroT3_Child_IDLE_08067691),
    PROC_END,
};

asm(".global gUnknown_08580FCC\n.set gUnknown_08580FCC, ProcScr_IntroT3Child\n");
