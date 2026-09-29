#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043FD8.
 * sub_08043FD8 @ 0x08043FD8, sub_0804402C @ 0x0804402C
 */

/* Matched wave 43 (W43-J), after three waves parked at 50.0%. Two facts, both
 * unreachable from the four-fresh-locals draft this replaces:
 *  - the results are written BACK to the parameters, as the already-matched
 *    sibling sub_08043C28 does. That is what makes agbcc keep the incoming y
 *    in r7 and emit the ROM's push {r4, r5, r6, r7, lr};
 *  - the low half must be the OR's LEFT operand. With fresh locals the two
 *    orders compile identically (W43-H measured that correctly); with the
 *    write-back they differ by a four-register rotation.
 * See the W43-J chapter in docs/agbcc-codegen.md. */

void sub_08043FD8(int x, int y, int oam2, int layer)
{
    u16 xh = x & ~0x1ff;
    u16 yh = y & ~0xff;

    x &= 0x1ff;
    y = (y - 0x30) & 0xff;

    PutSpriteExt(layer, x | xh, y | yh, gUnknown_084A0790, oam2);
}

/* Matched wave 43 (W43-J). Twin of sub_08043FD8 -- same shape, `- 0x10` and
 * gUnknown_084A07DA. Matched first try from that function's derivation; see
 * its comment for why the write-back to the parameters is what closed both. */

void PutCoMinimugSprite(int x, int y, int oam2, int layer)
{
    u16 xh = x & ~0x1ff;
    u16 yh = y & ~0xff;

    x &= 0x1ff;
    y = (y - 0x10) & 0xff;

    PutSpriteExt(layer, x | xh, y | yh, gUnknown_084A07DA, oam2);
}
asm(".global sub_0804402C\n.thumb_set sub_0804402C, PutCoMinimugSprite\n");
