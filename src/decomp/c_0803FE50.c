#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803FE50.
 * sub_0803FE50 @ 0x0803FE50
 */

/* Copies two 0x80-byte tile runs into VRAM, but only on every eighth id and
 * only while FindInventionOfKind reports a live record. The `(a2 & 0x18) >> 3` index
 * is an ARITHMETIC shift, so the expression is `int` rather than unsigned; the
 * 10-bit masks and the `<< 5` are 32-byte tiles. */
void AnimateVolcanoTiles(int a1, int a2)
{
    int t;

    if ((a2 & 7) == 0)
    {
        t = gUnknown_0849FAD8[(a2 & 0x18) >> 3] * 8;

        if (FindInventionOfKind(2) != NULL)
        {
            RegisterDataMove(gUnknown_081245F8 + ((t & 0x3FF) << 5),
                         (void *)(0x06010000 + (((a1 + 0xEA) & 0x3FF) << 5)), 0x80);
            RegisterDataMove(gUnknown_081245F8 + (((t + 4) & 0x3FF) << 5),
                         (void *)(0x06010000 + (((a1 + 0xF2) & 0x3FF) << 5)), 0x80);
        }
    }
}
asm(".global sub_0803FE50\n.thumb_set sub_0803FE50, AnimateVolcanoTiles\n");
