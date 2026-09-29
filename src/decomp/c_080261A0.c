#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080261A0.
 * sub_080261A0 @ 0x080261A0
 */

#include "unknown-functions.h"

int GetUnitSheetFrameTileCount(void)
{
    return 0x6c;
}
asm(".global sub_080261A0\n.thumb_set sub_080261A0, GetUnitSheetFrameTileCount\n");
