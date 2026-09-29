#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EED4.
 * sub_0803EED4 @ 0x0803EED4
 */

/* Reached through gMap (include/map.h) rather than a local struct cast: a
 * member access computes base + offsetof first and then adds the scaled
 * index, which is the ROM's order -- see c_0800B1FC.c for the fuller version
 * of this note.
 *
 * The second index is written `rowOffset[..] + (unk00 + pos.unk00)` with the
 * parenthesis load-bearing: -fforce-addr materialises the rowOffset element's
 * ADDRESS when the left operand is expanded and defers the `ldrh` to the add,
 * so the two byte reads land between the address and the load exactly as the
 * ROM has them. Without the parentheses the sum reassociates and the `ldrh`
 * moves four instructions earlier. */
u8 GetTerrainTypeAt(int a1, int a2)
{
    struct Unk02028360Pos pos;
    struct Unk02028360 *p;
    u8 ret;

    ret = gMap->terrain[gMap->rowOffset[a2] + a1];
    p = FindInventionAt(a1, a2);
    if (p != NULL)
    {
        GetInventionOriginOffset(p->unk02_6, &pos);
        ret = gMap->terrain[gMap->rowOffset[p->unk01 + pos.unk02] + (p->unk00 + pos.unk00)];
    }

    return ret;
}
asm(".global sub_0803EED4\n.thumb_set sub_0803EED4, GetTerrainTypeAt\n");
