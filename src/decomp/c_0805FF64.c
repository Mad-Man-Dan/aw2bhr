#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FF64.
 * sub_0805FF64 @ 0x0805FF64
 */

/* Five statements, no control flow: hand the current gUnknown_030040D8 record to
 * CreateMoveSlideForActiveUnit (whose ProcPtr result is discarded), then a copy from
 * gUnknown_030046CC into gUnknown_03003110 and a hand-off of the destination.
 *
 * Both buffer arguments are ADDRESSES -- `ldr rN, =sym` with no following load
 * -- so they are arrays, not pointer globals. gUnknown_03003110's address is
 * parked in r4 across the two calls because it is used by both; that is
 * allocation, not a source-level local.
 *
 * `strh` of 2 into gUnknown_030045D4, which is already typed u16.
 */

void AiExecutorStartMoveSlide(void)
{
    CreateMoveSlideForActiveUnit(gUnknown_030040D8);
    RebuildMapUnitLayers();
    UnpackPathNibbles(gUnknown_030046CC, gUnknown_03003110);
    StartUnitMoveSlide(gUnknown_03003110);
    gUnknown_030045D4 = 2;
}
asm(".global sub_0805FF64\n.thumb_set sub_0805FF64, AiExecutorStartMoveSlide\n");
