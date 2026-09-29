#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803DF54.
 * sub_0803DF54 @ 0x0803DF54
 */

/* SEPARATE `if`s, not one `&&` chain. The five tests cross-jump to a single
 * `movs r0,#0` either way, but the chain spelling lets gcc coalesce the
 * FindInventionAt result with GetInventionTargetCell's first argument and drops the
 * `adds r0,r4,#0` the ROM has -- two bytes. */
struct Unk02028360 *FindLivingInventionTargetAt(int a, int b)
{
    struct Unk02028360 *p;
    struct Unk02028360Pos v;

    p = FindInventionAt(a, b);
    if (p == NULL)
        return NULL;
    if (!GetInventionTargetCell(p, &v))
        return NULL;
    if (v.unk00 != a)
        return NULL;
    if (v.unk02 != b)
        return NULL;
    if (p->unk04 == 0)
        return NULL;
    return p;
}
asm(".global sub_0803DF54\n.thumb_set sub_0803DF54, FindLivingInventionTargetAt\n");
