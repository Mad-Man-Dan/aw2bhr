#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080466DC.
 * sub_080466DC @ 0x080466DC
 */

#include "hardware.h"

/* gUnknown_0812A10C IS NOT AN OBJECT. It is an agbcc `-fforce-addr` .rodata
 * word holding &gUnknown_084C1430, and every apparent second arrow in this
 * function is that indirection: `ldr rN,=gUnknown_0812A10C; ldr r4,[rN]` loads
 * the ADDRESS of the pointer variable, and `ldr r1,[r4]` is the ONLY real
 * dereference. Modelling it as a pointer-to-a-struct-holding-a-pointer (which
 * include/unknown-globals.h did until wave 34) costs one extra `ldr` at each of
 * the three read sites. The object is the same one src/decomp/c_080466A4.c
 * walks -- unk50 sits in the same record as its unk58/unk59. */
void IntelStatus_Loop(void)
{
    IntelStatus_StepPhase();

    if (gUnknown_084C1430->unk50 != 0)
    {
        sub_08046030();
        BG_EnableSyncBG0();
        BG_EnableSyncBG2();
        gUnknown_084C1430->unk50 = 0;
    }
    else if (gpKeySt->pressed & 3)
    {
        sub_08014878();
        PlayMusicOrSfx2(0x66);
        ClearSlotScriptCallback(gUnknown_03001FBC);

        if (gpKeySt->pressed & 2)
            gUnknown_03002F1C = 1;

        return;
    }

    if (gUnknown_084C1430->unk50 == 0)
        DrawIntelStatusArmyIcons();
}
asm(".global sub_080466DC\n.thumb_set sub_080466DC, IntelStatus_Loop\n");
