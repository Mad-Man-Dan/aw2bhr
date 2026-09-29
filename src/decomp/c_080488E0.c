#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080488E0.
 * sub_080488E0 @ 0x080488E0
 */

/* MATCHED byte-for-byte (wave 48, W48-I).
 *
 * PROMOTION NEEDS A .rodata POOL WORD PLACED:
 *   "rodata": ["0x0812A148"]   (then split_rodata.py + gen_lds.py)
 * 0x0812A148 is an agbcc -fforce-addr address constant holding 0x084C30F8,
 * i.e. &gUnknown_084C30F8. It is NOT an object; the honest spelling produces
 * it. Note the SAME global is reached two ways in this one function: through
 * that .rodata word in the prologue/switch, and through a plain inline pool
 * word inside the loop.
 *
 * TWO ASSIGNMENT-EXPRESSIONS ARE LOAD-BEARING, and both are the wave-17
 * "create the pseudo at a point no statement boundary can reach" lever. They
 * are not stylistic and the function does not match without them:
 *
 *   (c = 0xa0)      forces the 0xa0 constant's pseudo to be created inside the
 *                   first store's index. Without it agbcc allocates the
 *                   constant and the destination pointer to swapped registers
 *                   and loads the pointer after the index instead of before
 *                   (13 bytes, size already exact). decomp-permuter found this
 *                   one in 300 s / ~18,800 iterations from a 90.7% draft.
 *
 *   (m = n + 1)     forces the loop-invariant `n + 1` to be created AFTER the
 *                   two LICM-hoisted global addresses rather than before them.
 *                   This is the whole of the last 13 bytes.
 *
 * The second is the transferable finding and it REFUTES the naive reading of
 * the preheader three-pass rule. `n + 1` is emitted LAST in the preheader, i.e.
 * after the LICM address hoists. Authoring `m = n + 1;` as a statement -- before
 * the `for`, in the `for` init, at the top of the outer body, after the inner
 * loop, or between the two stores inside the inner loop -- puts it FIRST in the
 * preheader in EVERY one of those five positions (measured, compile_probe).
 * Source statement POSITION does not control where a hoisted invariant lands.
 * What does is where the pseudo is CREATED in the expression tree: written as
 * an assignment-expression inside the second store's index, `n + 1` is created
 * after both address references and hoists after them.
 *
 * Also measured and ruled out for `(y + (n + 1)) * 32`:
 *   (y + n + 1)   -> shares `y + n`, no invariant at all, -16 bytes
 *   (y + (n + 1)) -> fold rewrites to `(y + 1) + n`, -12 bytes
 *   (n + 1 + y)   -> identical to the above; fold canonicalises them
 * The ROM's `sub sp, #4` is `n + 1` SPILLED, and an unexplained `sub sp, #N`
 * plus a misplaced constant add really are one fact.
 *
 * What the function does: the switch on unk030 falls 3 -> 2 (case 3 decrements
 * v first, then does case 2's work), then it blits two 20-column rows per pass
 * out of gUnknown_084C30F8->unk032 into *gBG0TilemapBuffer at rows +0xa0/+0xc0,
 * three passes with y = 2, 4, 6. The destination is `y * 32 + x + 0xa0`, not
 * `(y + 5) * 32 + x`: the ROM adds 0x140/0x180 as BYTE constants to one shared
 * `y * 32 + x`.
 */

void ShopList_StepScroll(void)
{
    u16 n;
    u16 v;
    u16 x;
    u16 y;
    int m;
    int c;

    v = gUnknown_084C30F8->unk020;
    if (gUnknown_084C30F8->unk030 == 0)
        return;

    n = 0;
    switch (gUnknown_084C30F8->unk030)
    {
    case 1:
        gUnknown_084C30F8->unk030 = 0;
        break;
    case 3:
        v--;
    case 2:
        n++;
        gUnknown_084C30F8->unk030 = 1;
        break;
    }

    RenderShopListBuffer(n, v);

    for (y = 2; y <= 7; y += 2)
    {
        for (x = 0; x <= 0x13; x++)
        {
            gBG0TilemapBuffer[y * 32 + x + (c = 0xa0)] =
                gUnknown_084C30F8->unk032[(y + n) * 32 + x];
            gBG0TilemapBuffer[y * 32 + x + 0xc0] =
                gUnknown_084C30F8->unk032[(y + (m = n + 1)) * 32 + x];
        }
    }

    BG_EnableSyncBG0();
}
asm(".global sub_080488E0\n.thumb_set sub_080488E0, ShopList_StepScroll\n");
