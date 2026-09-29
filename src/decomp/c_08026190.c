#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026190.
 * sub_08026190 @ 0x08026190, sub_08026198 @ 0x08026198
 */

u8 *GetUnitSheetGraphics(void)
{
    return gUnknown_0810BE60;
}
asm(".global sub_08026190\n.thumb_set sub_08026190, GetUnitSheetGraphics\n");

u8 *GetUnitExtraGraphics(void)
{
    return gUnknown_0810E820;
}
asm(".global sub_08026198\n.thumb_set sub_08026198, GetUnitExtraGraphics\n");
