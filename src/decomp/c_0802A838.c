#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A838.
 * sub_0802A838 @ 0x0802A838, GetTerrainNameGraphic @ 0x0802A85C
 */

const u8 * GetUnitNameGraphic(int a1)
{
    return gUnknown_08108264[(gUnknown_0849A354[a1].unk00 * 8) & 0x3ff];
}
asm(".global sub_0802A838\n.thumb_set sub_0802A838, GetUnitNameGraphic\n");

const u8 * GetTerrainNameGraphic(int a1)
{
    return gUnknown_08106A64[(gUnknown_0849A2C8[a1].nameGraphic * 8) & 0x3ff];
}

asm(".global sub_0802A85C\n.thumb_set sub_0802A85C, GetTerrainNameGraphic\n");
