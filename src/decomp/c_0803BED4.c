#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BED4.
 * sub_0803BED4 @ 0x0803BED4
 */

/* Four sequential statements, every result discarded. */
void sub_0803BED4(void)
{
    SetupBackgrounds(gUnknown_0849D16C);
    LoadCursorSpriteGraphics();
    LoadBg1WindowFrame(0);
    sub_08037F18();
}
