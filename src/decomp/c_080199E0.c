#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080199E0.
 * sub_080199E0 @ 0x080199E0, sub_080199EC @ 0x080199EC, sub_080199F8 @ 0x080199F8
 */

#include "hardware.h"

void StashMapId(void)
{
    gPlaySt.unk03 = gPlaySt.mapID;
}
asm(".global sub_080199E0\n.thumb_set sub_080199E0, StashMapId\n");

void RestoreStashedMapId(void)
{
    gPlaySt.mapID = gPlaySt.unk03;
}
asm(".global sub_080199EC\n.thumb_set sub_080199EC, RestoreStashedMapId\n");

void DisableWindow0(void)
{
    gDispIo.disp_ct.win0_enable = FALSE;
}
asm(".global sub_080199F8\n.thumb_set sub_080199F8, DisableWindow0\n");
