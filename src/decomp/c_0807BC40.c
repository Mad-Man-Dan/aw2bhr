#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BC40.
 * sub_0807BC40 @ 0x0807BC40
 */

#include "hardware.h"

/* MissionTitleName_SetupBlend @ 0x0807BC40, 176 bytes, THUMB. Matched.
 *
 * Same shape as SuperCoPowerBg_Init minus the trailing gDispIo/SetDispEnable
 * run (data_refs excludes gDispIo here) -- see that function's comment
 * for the shared idiom. Only the constants differ: EVA/EVB are both
 * 0x10 here (vs. 0/0x10), the target1/target2 group bits set are bg2
 * and bg1 instead of bg0 and bg3, and the 03001FE8/030030B4 priority
 * values are swapped (3 then 2, vs. 2 then 3) -- which is why the first
 * gets a bare all-ones orr and the second needs the AND+OR pair.
 */
void MissionTitleName_SetupBlend(void)
{
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = gUnknown_03001FFC;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xFFE0) | 4;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x200;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 3;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 1;
}
asm(".global sub_0807BC40\n.thumb_set sub_0807BC40, MissionTitleName_SetupBlend\n");
