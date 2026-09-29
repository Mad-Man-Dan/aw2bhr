#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804DA40.
 * sub_0804DA40 @ 0x0804DA40
 */

/* MATCHED wave 86 (W86-B), first attempt, after parks in waves 34, 35, 59, 79.
 *
 * THE FIX IS A TYPE, NOT AN ALLOCATION LEVER: `d` is u16, not u32. With
 * `u16 d; u16 e; u16 f; u16 side;` and the twin's statement order this is
 * byte-for-byte identical. Nothing else changed.
 *
 * THE PARK'S CLASSIFICATION WAS WRONG AND SO WAS ITS ORACLE. The wave-79
 * header called this "NO SOURCE CONSTRUCT -- three values with identical
 * reference counts tie on allocno priority, register numbers with nothing
 * behind them; do not give it another budget without a NEW mechanism." There
 * was a source construct and it was the declared width of `d`.
 *
 * The reasoning that pinned `d` to u32 was inverted. The park said:
 *   "`d` is u32, NOT the u16 c_0804E4CC.c uses, and the DIVISION is the
 *    oracle: the ROM calls __udivsi3/__umodsi3, where a u16 `d` promotes to
 *    signed int and gives __divsi3/__modsi3."
 * A u16 `d` does NOT give signed division here. c-typeck's build_binary_op
 * sets `shorten` for TRUNC_DIV_EXPR/TRUNC_MOD_EXPR when operand 0 is a
 * NOP_EXPR from an unsigned type AND operand 1 is an INTEGER_CST that is not
 * all-ones -- both hold for `d / 0x38` on a u16 `d`. shorten_binary_op then
 * performs the division in the shortened UNSIGNED type, so the ROM's
 * __udivsi3/__umodsi3 is exactly what u16 emits. The promotion-to-signed-int
 * rule the park invoked is real C, but shorten_binary_op runs after it and
 * undoes it for a constant divisor. u32 also emits __udivsi3, which is why
 * the wrong width was invisible to that oracle -- it was byte-neutral on the
 * CALL and cost a register in the high bank.
 *
 * HOW IT WAS FOUND -- the wave-77 grep-the-tree rule, applied to a twin the
 * park never named. The park listed FigureTileHook_CruiserVariant2 and FigureTileHook_Cruiser as "the
 * twins" and recorded that nothing in either transfers. The actual nearest
 * matched relative is LanderPart_StreamHook (src/decomp/c_0804CE24.c): same
 * prototype, same CopySlotSpriteAttrs readout, the SAME 0x38 divisor and the SAME
 * 0x700 stride, differing only in the gUnknown_02029668 column ([4] vs [3])
 * and the VRAM base (0x06011000 vs 0x06010C00). It has been matched with
 * `u16 d` the whole time. Reading it settled a four-wave park in one probe.
 *
 * LESSON FOR THE NEXT WAVE: "the twins" in a park entry is a claim about
 * which functions were looked at, not about which functions are nearest.
 * Re-derive the twin list from the CONSTANTS (divisor, stride, VRAM base),
 * not from the address neighbourhood -- LanderPart_StreamHook is 0xC1C bytes away
 * from this function while the named twins are adjacent.
 *
 * SETTLED and still true from the earlier parks:
 *   - The prototype is (s16, u16 *), not `void (void)`.
 *   - `oam.tileNum` must be read TWICE and must not be bound to a local
 *     (wave 79 measured -8 for binding it).
 *   - gUnknown_02029668 is `s16 [][5]` and this reads column 3.
 *   - The destination base 0x06010C00 is the folded 0x06010000 + 0xC00.
 *   - gUnknown_08552FB0 is `void *[]`, a LOADED pointer table, so the
 *     wave-51 StartWholeFigureSlot ARRAY_REF-on-the-bare-symbol lever that the park
 *     pointed at has no analogue here: there is no address-of-a-symbol to
 *     route through get_inner_reference, only a pointer fetched from memory.
 *     Three matched files (c_0804C8C8.c, c_0804CE24.c, c_0804E4CC.c) already
 *     use this exact `(u8 *)tbl[i] + k * S` cast spelling. Checked and
 *     refuted this wave; do not spend a probe on it.
 */
void CruiserPart2_StreamHook(s16 a, u16 *p)
{
    struct OamData oam;
    u16 d;
    u16 e;
    u16 f;
    u16 side;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);
    d = (p[2] - oam.tileNum) & 0x3ff;
    e = d / 0x38;
    f = d % 0x38;
    side = gUnknown_03001470[a].unk30;
    p[2] = (p[2] & 0xfc00) + f + oam.tileNum;

    if (a == gUnknown_02029668[side][3] && d != gUnknown_03001470[a].unk28)
        RegisterDataMove((u8 *)gUnknown_08552FB0[side] + e * 0x700,
                     (void *)(0x06010C00 + (side << 13)), 0x700);
}
asm(".global sub_0804DA40\n.thumb_set sub_0804DA40, CruiserPart2_StreamHook\n");
