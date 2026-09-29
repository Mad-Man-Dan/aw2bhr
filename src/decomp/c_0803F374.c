#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F374.
 * sub_0803F374 @ 0x0803F374
 */

/* MATCHED. A LOOSE duplicate of LoadMainMenuLabelPlateGraphic -- one Decompress call, a
 * different blob and a different VRAM destination. */
void sub_0803F374(void)
{
    Decompress(gUnknown_081169D0, (void *)0x06013940);
}
