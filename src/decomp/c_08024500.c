#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024500.
 * sub_08024500 @ 0x08024500
 */

#include "hardware.h"

/* Sibling of SetMapLayersBg0BlendFade / SetMapLayersUnitsTranslucent -- same shape, different priorities,
 * different BLDCNT target2 group and different EVA. See the note in
 * work/SetMapLayersBg0BlendFade. The two 0x10 coefficient stores share one `movs r1, #0x10`
 * because both shadows take the same value; that is CSE, not one statement.
 */

void SetMapLayersTargetBlend(void)
{
    gUnknown_03002B6C.bits.priority = 2;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 0;
    gUnknown_0300251C.bits.priority = 3;
    SetDefaultColorEffects();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0x10;
    ResetWindowShadows();
}
asm(".global sub_08024500\n.thumb_set sub_08024500, SetMapLayersTargetBlend\n");
