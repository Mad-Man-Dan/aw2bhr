#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A440.
 * sub_0803A440 @ 0x0803A440
 */

/* src/decomp/c_0803A53C.c's shape twice over -- see the note there for why the
 * callback goes through a `(void *)` cast. */
void UnitInfoPanel_AddDrawCallbacks(void)
{
    RunOrQueueDrawCallback((void *)UnitInfoPanel_DrawPictureAndFuel, 1);
    RunOrQueueDrawCallback((void *)UnitInfoPanel_DrawAmmoAndRange, 1);
}
asm(".global sub_0803A440\n.thumb_set sub_0803A440, UnitInfoPanel_AddDrawCallbacks\n");
