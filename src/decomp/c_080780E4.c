#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080780E4.
 * sub_080780E4 @ 0x080780E4
 */

#include "hardware.h"
#include "proc.h"

/* A mode-entry sequence: install the two DefaultVBlankCallback / DefaultMainLoopCallback handlers
 * through the setter pair documented on sub_080366C4 / sub_080366D0 in
 * include/unknown-functions.h, run three more `void (void)` leaves, drop
 * forced blank, and start gUnknown_086147FC on tree 3.
 *
 * `ldrb; movs #0x7f; ands; strb` on gDispIo's first byte is the DISPCNT shadow
 * bitfield at bit 7, i.e. forced_blank cleared -- a byte-wide read-modify-write
 * of the low half of a `struct DispCnt`, which is what the bitfield write emits
 * and what a `u16`-wide spelling would not. */

void BootIntoWorldMap(void)
{
    EnableSpriteLayerMode();
    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
    InitGameSystems();
    ClearWorldMapMarkers();
    gDispIo.disp_ct.forced_blank = 0;
    Proc_Start(gUnknown_086147FC, PROC_TREE_3);
}
asm(".global sub_080780E4\n.thumb_set sub_080780E4, BootIntoWorldMap\n");
