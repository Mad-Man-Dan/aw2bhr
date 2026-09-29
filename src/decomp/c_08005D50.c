#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08005D50.
 * sub_08005D50 @ 0x08005D50
 */

/* The deref stays live across all three stores, so the source held it in a
 * local; spelled `gActiveMap->unkNN` throughout it reloads between the
 * stores, as its neighbour DesignRoomResetArmyPanels does.
 */
void sub_08005D50(void)
{
    struct ActiveMap *p = gActiveMap;

    p->overlayX = 0x100;
    p->overlayY = 0x800;
    p->overlayState = 0;
}
