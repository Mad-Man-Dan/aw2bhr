#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038BE0.
 * sub_08038BE0 @ 0x08038BE0
 */

void RebuildBestMovePath(void)
{
    TruncateMovePath(1);
    FillMovementMapFromMovePathEnd();
    GenerateBestMovementScript(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02, gUnknown_03003110);
    RebuildMovePathFromDirections();
}
asm(".global sub_08038BE0\n.thumb_set sub_08038BE0, RebuildBestMovePath\n");
