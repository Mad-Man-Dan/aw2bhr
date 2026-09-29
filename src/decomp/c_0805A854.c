#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A854.
 * sub_0805A854 @ 0x0805A854
 */

/* AiPickShoalParkBeside's sibling against sub_0805C128, differing in that the scratch
 * cell is a LOCAL rather than the caller's, and the accepted cell is written
 * back over the input pair.
 *
 * THE SENTINEL MUST NOT BE BOUND HERE -- the opposite of AiPickShoalParkBeside and of
 * AiPickFiringCellBesideUnit, and the local is what flips it. `pos.x` is a halfword member
 * of a four-byte struct, i.e. an SImode aggregate, so the seed store is a
 * bitfield insert: `ldr [sp]; and #0xFFFF0000; orr #0x270F; str [sp]`. The
 * 0x270F has to be an operand of that `orr`, so its `ldr` belongs BETWEEN the
 * `and` and the `orr`; a binding statement emits it before the `and` instead
 * and swaps the two pool words. CSE still parks the constant in r4 for the
 * final compare, so nothing is lost by spelling it twice.
 *
 * The tail is `== -> 0` for the reason recorded on AiPickShoalParkBeside: the `if` body
 * is the block laid out after the literal pool, and the ROM's `beq` reaches
 * `movs r0,#0`. */
struct CellXY
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};

int sub_0805A854(u16 * p)
{
    struct CellXY pos;

    pos.x = 0x270F;

    sub_0805C128(p[0] - 1, p[1], (u16 *)&pos);
    sub_0805C128(p[0] + 1, p[1], (u16 *)&pos);
    sub_0805C128(p[0], p[1] - 1, (u16 *)&pos);
    sub_0805C128(p[0], p[1] + 1, (u16 *)&pos);

    if (pos.x == 0x270F)
        return 0;

    p[0] = pos.x;
    p[1] = pos.y;
    return 1;
}
