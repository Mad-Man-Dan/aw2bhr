#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080399F8.
 * sub_080399F8 @ 0x080399F8
 */

#include "hardware.h"

/* The general form of src/decomp/c_08080498.c's last two statements: that
 * function is this one with a == 0x2B0 and b == 8 folded into literals
 * (0x2B0 * 0x20 == 0x5600, and (8 << 12) | 0x2B0 == 0x82B0), which is why its
 * data_refs are a superset of these.
 *
 * The three-term address expression only matches in ONE order, and the rule is
 * worth keeping: agbcc evaluates the two non-constant terms in SOURCE order and
 * folds the literal into the SECOND one. The ROM computes the chr_block term
 * first and adds 0x06000000 to the tile term, so the tile term has to come
 * last. `0x06000000 + tile + chr` (the obvious spelling) computes the tile term
 * first and attaches the constant to chr -- the exact mirror image -- and
 * `chr + tile + 0x06000000` emits the constant as a third, separate add.
 *
 * Both parameters are WIDE: bare `adds r5, r0, #0` / `adds r4, r1, #0` with no
 * PROMOTE_MODE pair at entry. The `lsls #0x10; lsrs #0x10` on the last argument
 * is a narrowing at the USE, and it is evidence that sub_08012B70's fifth
 * parameter is really `u16` rather than the `int` it is declared with; the cast
 * is written here rather than retyping the prototype, because that would need
 * every other caller re-verified and this wave did not do that. The two
 * spellings are byte-identical at this call site.
 *
 * Wave 39 (W39-A) did the retype: sub_08012B70's last three arguments are u16
 * and the callee's own prologue proves it. All three callers were re-run
 * through trymatch and all three still match, so the cast below is now
 * redundant -- it is left in place because it is byte-identical either way and
 * removing it would be churn on a matched function. */
void LoadCoPowerPanelTiles(int a, int b)
{
    RegisterDataMove(gUnknown_080A29A4,
                 (void *)(0x06000000 + gUnknown_03002B6C.bits.chr_block * 0x4000
                          + (a & 0x3ff) * 0x20),
                 0x800);
    sub_08012B70(gBG0TilemapBuffer, gUnknown_080A31A4, 0, 0, (u16)(b << 12 | a));
}
asm(".global sub_080399F8\n.thumb_set sub_080399F8, LoadCoPowerPanelTiles\n");
