#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080393AC.
 * sub_080393AC @ 0x080393AC
 */

/* Both callees take an int, so the s16 global is loaded `ldrsh` twice through
 * a scratch register -- ldrsh has no immediate-offset form. */

void sub_080393AC(void)
{
    EnableSlotSpriteAffine(gUnknown_03001FBC);
    SetSlotSpriteDoubleSize(gUnknown_03001FBC);
}
