#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C268.
 * sub_0804C268 @ 0x0804C268, sub_0804C340 @ 0x0804C340
 */

/* MATCHED wave 51 (W51-D). Twin of StartWholeFigureSlot -- read that file header for
 * the fix, which is the same one and closed both functions in a single edit:
 * gUnknown_085D6A48 must be reached as an ARRAY_REF on the bare symbol
 * (gUnknown_085D6A48[0][j * 4]), not through a cast, so that expand_expr takes
 * the get_inner_reference path and emits the base before the offset.
 * See docs/agbcc-codegen.md.
 *
 * SETTLED:
 *   - Parameters are (u16, u16); the entry lsls/lsrs #0x10 pairs are
 *     PROMOTE_MODE on declared-narrow parameters.
 *   - Argument 5 is (u8)n on an int n, NOT a u8 n. Both emit the same
 *     lsls/lsrs #24 pair but in DIFFERENT PLACES: u8 n truncates at the
 *     definition, right after the adds r4,#4, while the ROM truncates at the
 *     use, in the four instructions before str r4,[sp]. Wave 15 probe blind
 *     spot on a real function.
 *   - j must be int (see the twin).
 *   - Argument 3 is gUnknown_02029BA8[a].unk04, argument 4 is .unk00.
 *   - gUnknown_08552178 is u16[][5]: the index is a*5 + b built as
 *     lsls #2; add; add and then scaled by 2.
 */
void StartFigureSlot(u16 a, u16 b)
{
    u16 i;
    int j;
    int n;
    void *q;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    i = gUnknown_03004580[a][2];
    j = gUnknown_03004580[a][1] * 3;
    n = gUnknown_08552178[a][b] * 7 + 4;
    q = gUnknown_02029BA8[a].unk00;

    gUnknown_02029A10[a].entries[b].unk18 = sub_08015410(
        (void *)gUnknown_08552FB8[gUnknown_085D6A48[0][j * 4]][i][0],
        1,
        gUnknown_02029BA8[a].unk04,
        q,
        (u8)n);
}
asm(".global sub_0804C268\n.thumb_set sub_0804C268, StartFigureSlot\n");

/* MATCHED wave 51 (W51-D). Twin of StartFigureSlot -- same function, different
 * arguments; one substitution closed both.
 *
 * THE FIX, after sixteen waves at 97.9%: the ROM materialises the
 * gUnknown_085D6A48 base BEFORE the scaled index, which no cast of the symbol
 * can produce. A cast makes the address a PLUS_EXPR, and fold canonicalises
 * PLUS(addr_const, j * 8) so the MULT becomes operand 0 and is expanded first.
 * A subscript on the bare VAR_DECL stays an ARRAY_REF, which expand_expr routes
 * through get_inner_reference -- base object first, offset second,
 * unconditionally. gUnknown_085D6A48 is u16[][12], so the 8-byte-stride view is
 * spelled gUnknown_085D6A48[0][j * 4]. See docs/agbcc-codegen.md.
 * The four spellings the old note ruled out were all pointer arithmetic, which
 * is why they measured identical to each other.
 *
 * SETTLED:
 *   - PARAMETER 2 IS NEVER READ. r1 is dead from entry; the body recomputes
 *     gUnknown_08552148[a]. The parameter stays because the caller passes it.
 *   - Parameters are (u16, u16): the opening lsls/lsrs #0x10 pair is
 *     PROMOTE_MODE on a declared-narrow parameter. int emits no shift there.
 *   - j must be int. As u16 the * 3 is truncated at the definition and the
 *     * 8 folds into the reload as lsrs #13 -- one instruction too many.
 *   - Argument 5 is a named local holding literal 0, materialised early
 *     (movs r2,#0; mov sl,r2) and held across fifteen instructions. An inline
 *     literal cannot do that.
 *   - The body reads gUnknown_0300453C and gUnknown_0300451C BACK rather than
 *     using the parameters, both before the call and again after it.
 */
void StartWholeFigureSlot(u16 a, u16 b)
{
    u16 i;
    int j;
    int n;
    void *q;

    gUnknown_0300453C = a;
    gUnknown_0300451C = gUnknown_08552148[a];

    i = gUnknown_03004580[gUnknown_0300453C][2];
    j = gUnknown_03004580[gUnknown_0300453C][1] * 3;
    n = 0;
    q = gUnknown_02029BA8[gUnknown_0300453C].unk00;

    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk18 =
        sub_08015410(
            (void *)gUnknown_08552FB8[gUnknown_085D6A48[0][j * 4]][i][0],
            1,
            gUnknown_02029BA8[gUnknown_0300453C].unk04,
            q,
            n);
}
asm(".global sub_0804C340\n.thumb_set sub_0804C340, StartWholeFigureSlot\n");
