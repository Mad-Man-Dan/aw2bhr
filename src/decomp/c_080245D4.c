#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080245D4.
 * sub_080245D4 @ 0x080245D4
 */

#include "hardware.h"

/* Sibling of SetMapLayersBg0BlendFade / SetMapLayersTargetBlend -- same shape. Here target1 is BG1,
 * so the constant reused by the byte-0 store is the 2 left in r4 by the
 * gUnknown_03001FE8 priority write rather than the 1 from gUnknown_030030B4;
 * that is why gUnknown_030030B4's `movs #1` lands in the scratch r0 and the
 * push list is only {r4, lr}. See the note in work/SetMapLayersBg0BlendFade.
 */

void SetMapLayersUnitsTranslucent(void)
{
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 1;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;
    gUnknown_03002020 = 6;
    gUnknown_03002B28 = 0x10;
    ResetWindowShadows();
}
asm(".global sub_080245D4\n.thumb_set sub_080245D4, SetMapLayersUnitsTranslucent\n");
