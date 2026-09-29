#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057860.
 * sub_08057860 @ 0x08057860
 */

/* MATCHED in Wave 72, configured, 344/344 bytes and relocations exact.
 * PARKED at 95.6% before Wave 63. Wave 63 improved this to 98.0%, size EXACT
 * (344 bytes), 7 of 344 bytes differing, first difference at +0xb4, by fixing
 * the early pool-load
 * placement by binding the i * sizeof(struct Unk8553A1C) byte offset before
 * forming p and q.
 *
 * The old r8/r9 diagnosis is STALE: the current draft already parks `dst` in
 * r9 (sb) and `m` in r8, exactly like the ROM.
 * What remains after the Wave 63 head fix is only:
 *  - the two DESCENDING loops (the j != 0 arm) build their start address as
 *    `offset + dst` where the ROM builds `dst + offset`
 *    (`adds r1,r0,r3` vs the ROM's `adds r1,r3,r0`, and
 *    `lsls;mov;adds #0x40;adds` vs the ROM's `lsls;adds #0x40;mov;adds`).
 *    Both ASCENDING loops already match byte-for-byte, so it is specific to
 *    the reversed giv.
 *
 * Two spellings for the descending arm were TESTED and are worse, do not retry
 * them: a pointer walk (`q = dst + b; ... *q-- = v;`) hoists the address above
 * the zero-trip guard and adds two instructions, and `dst[b + 32 - k]` folds
 * `(b + 32) * 2` instead of distributing it. `dst[b - k + 32]` -- used here --
 * is the spelling that gets the instruction COUNT right.
 * 300 s of decomp-permuter (~15k iterations) took this from 93.0% to 95.6% and
 * contributed the `new_var` / `k` temporaries below; they are permuter output,
 * not a reading of the original, but they are worth 2.6%. A second 300 s pass
 * from the Wave 63 98.0% draft tried 15,041 candidates without improvement.
 *
 * The head IS settled and cost most of the work. gUnknown_08553A2C and the
 * pair four bytes further on are reached from ONE pool word, with the second
 * base arriving as `adds r0,#4`. That is cse.c's use_related_value, and it can
 * only fire on two constants derived from the SAME symbol, so:
 *   - declaring gUnknown_08553A30 as a second linker symbol emits a second
 *     pool word (tested: +4 bytes, 57.3%);
 *   - carving `s8 unk04; s8 unk05;` into struct Unk8553A1C reads them off a
 *     single base with `movs rI,#4 / ldrsb` instead of building base+4
 *     (tested);
 *   - `&gUnknown_08553A2C[i].filler_02[2]` splits the 4 as base+2 and index 2
 *     (tested).
 * The cast on the base pointer below is the one that produces the ROM's
 * `ldr =gUnknown_08553A2C / adds r6,r3,r0 / adds r0,#4 / adds r3,r3,r0`, and
 * it also fixes the ldrsb addressing mode on both pairs.
 *
 * Wave 72's one distinct scheduling lever was the direct pointer spelling for
 * the descending stores: `*(dst + b - k)` (and `+ 32`). Unlike the previously
 * tested pointer walk, this keeps the zero-trip guard and reversed index shape
 * intact while making agbcc emit the ROM's base-plus-offset operand order. */

struct Unk8057Pos
{
    u16 x;
    u16 y;
};

void sub_08057860(u16 *dst, int i, struct Unk8057Pos *pos)
{
    struct Unk8553A1C *p;
    struct Unk8553A1C *q;
    int new_var;
    int j;
    int a;
    int b;
    int n;
    int m;
    int k;
    int off;

    m = 0;
    j = i & 1;
    off = i * sizeof(struct Unk8553A1C);
    p = (struct Unk8553A1C *)((u8 *)gUnknown_08553A2C + off);
    q = (struct Unk8553A1C *)((u8 *)gUnknown_08553A2C + off + 4);
    a = (pos->x + p->unk00) + ((pos->y + p->unk01) << 5);
    b = (pos->x + q->unk00) + ((pos->y + q->unk01) << 5);
    n = Div(gUnknown_03004580[j][7], 10);

    if (GetPlayerSpecialAbilities(GetArmyByTeamColor(gUnknown_03004580[j][0] + 1)) & 0x20)
    {
        k = n - (n >> 1);
        m = k;
        n = n >> 1;
    }

    dst[a] = gUnknown_08562124[j] * 0x1000 + 0x11;
    dst[a + 1] = gUnknown_08562124[j] * 0x1000 + 0x12;

    if (j != 0)
    {
        new_var = n;
        for (k = 0; k < new_var; k++)
            *(dst + b - k) = gUnknown_08562124[j] * 0x1000 + 0x13;
        if (m != 0)
        {
            for (k = 0; k < m; k++)
                *(dst + b - k + 32) = gUnknown_08562124[j] * 0x1000 + 0x13;
        }
    }
    else
    {
        for (k = 0; k < n; k++)
            dst[b + k] = gUnknown_08562124[j] * 0x1000 + 0x13;
        if (m != 0)
        {
            for (k = 0; k < m; k++)
                dst[(b + 32) + k] = gUnknown_08562124[j] * 0x1000 + 0x13;
        }
    }
}
