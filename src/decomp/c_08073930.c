#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073930.
 * sub_08073930 @ 0x08073930
 */

/* MATCHED, wave 73 (W73-B). The residual was the ORDER of the two `ldrh`s plus
 * which register holds the row base, and the lever is a COMMA OPERATOR.
 *
 * The ROM evaluates the SUBTRAHEND first. A statement boundary
 * (`t = row[line].unk02;` on its own line) does produce the ROM's operand order
 * and registers, but it drags the 0x04000014 pool `ldr` down between the two
 * loads, because an address constant's `ldr` lands at the head of the statement
 * that first references it. The single-statement form is the mirror image:
 * right pool position, wrong operand order. The comma operator satisfies both
 * at once -- it creates the reference at a point no statement boundary can
 * reach, so the pool `ldr` stays at the head of the one statement while the
 * subtrahend is still evaluated first.
 *
 * Measured in one compile_probe, four spellings side by side:
 *   single statement        -> 85.3% (pool right, operands wrong)
 *   separate statement      -> pool `ldr` sinks between the loads
 *   comma in the RHS        -> EXACT (this file)
 *   comma inside operand 0's index (`row[(t = row[line].unk02, 0)]`) -> EXACT
 * Both comma depths give identical assembly, so the lever is the comma itself,
 * not the depth at which it sits.
 *
 * Toolchain axis measured and negative: no-force 29.4%, o1 41.2%,
 * o1-no-force 41.2%, old-agbcc 14.7%, old-agbcc-no-force 27.9%.
 *
 * The type model: the ROM word at 0x081CC02C holds 0x0202FDE4, so the
 * three-level load is agbcc's own -fforce-addr copy and NOT a pointer to a
 * pointer, and the row stride is four with the only live member at +2.
 */

#include "hardware.h"
/* The scanline record gUnknown_0202FDE4 points at while this handler runs:
 * stride FOUR, and only the halfword at +2 is read. The global is declared
 * `void *` in unknown-globals.h because CircleWipe_Loop's REG_DMA0SAD use of the
 * same slot needs no type; the cast lives here rather than in the header until
 * the two readings are reconciled. */
struct Unk73930Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
};

/* An HBlank handler: scroll BG1 by the difference between row 0's stored
 * offset and this scanline's, re-arming the row pointer from
 * gUnknown_0202FDDC once VCOUNT has run off the bottom.
 *
 * `movs r1, #0x80; lsls r1, r1, #1` is the constant 0x100. */

void PolygonWipe_HBlankHandler(void)
{
    u16 line = REG_VCOUNT + 1;
    u16 t;

    if (line > 0xa0)
    {
        gUnknown_0202FDE4 = gUnknown_0202FDDC;
        line = 0;
    }

    REG_BG1HOFS = (t = ((struct Unk73930Row *)gUnknown_0202FDE4)[line].unk02,
        ((struct Unk73930Row *)gUnknown_0202FDE4)[0].unk02 - t + 0x100);
}
asm(".global sub_08073930\n.thumb_set sub_08073930, PolygonWipe_HBlankHandler\n");
