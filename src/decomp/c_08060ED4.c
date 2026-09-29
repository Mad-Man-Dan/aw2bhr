#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060ED4.
 * CountBuildablePropertiesOfKind @ 0x08060ED4, AiCalcBuildPriorities @ 0x08060F00, AiMarkAffordableUnits @ 0x08060F74
 */

/* AiCalcBuildPriorities @ 0x08060F00, 116 bytes.
 *
 * Scores all 24 unit types into gUnknown_03004640: a type whose weight byte is
 * zero scores 0xFF (i.e. "never"), everything else scores
 * `CountUnitsOfType(type) * 1000 / gUnknown_03004674 * 10 / weight` -- the type's
 * own count worked up into a per-mille share of the AI's occupied-cell tally
 * and then divided by the weight. Both divides are `bl __divsi3`, the SIGNED
 * helper, which is what makes every operand here `int`.
 *
 * The two multiplications are SYNTHESISED. `lsls #5; subs; lsls #2; adds;
 * lsls #3` is ((x*32 - x)*4 + x)*8 = x * 1000 and `lsls #2; adds; lsls #1` is
 * x * 10; both come from the plain constants below, so do not author shifts.
 *
 * The table is the 12-byte-strided one *gUnknown_085766E0 addresses, reached
 * here at row+7 -- one byte past the `v[7]` array src/decomp/c_08061178.c
 * names in the same row, and indexed the same way, by `type - 1`. See the
 * CONFLICT note beside struct Unk085766E0 in include/unknown-globals.h: that
 * tag's declared extent cannot carry this stride, so the object is described
 * with a file-local view and cast onto the shared symbol, exactly as
 * c_08061178.c does.
 *
 * The `ldrh r0, [r5]` before the store, and the second one after the divide,
 * are the volatile-store artefact documented beside gUnknown_03004640: they
 * load nothing the function uses and there is no read in the source. Two of
 * them survive because the STORE is written twice, once per arm -- agbcc
 * cross-jumps the two identical `strh`s into one tail but cannot merge the
 * loads, whose destination registers differ. A single store fed by an `r`
 * local emits one `ldrh` at the merge and is 2 bytes short. */

struct Unk60F00Row
{
    /* 0x00 */ u8 v[7];
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u8 filler_08[4];
};
struct Unk60F00Tbl
{
    /* 0x00 */ u8 filler_00[0x14];
    /* 0x14 */ struct Unk60F00Row rows[1];
};

/* CountBuildablePropertiesOfKind @ 0x08060ED4, 44 bytes.
 *
 * Counts the entries of the 0xFF-terminated gUnknown_085766E4 list that name
 * class `a1` and have not been marked consumed. The 4-byte stride, the 0xFF
 * terminator on unk00 and the 0xFE "consumed" marker on unk03 are all
 * AiPickBestScoredBuildSite's -- see src/decomp/c_08061668.c, the writer this reader pairs
 * with, which sets unk03 = 0xFE on the record it claims.
 *
 * The walk is a POINTER local here where c_08061668.c needed the subscript
 * form: this ROM keeps only the walker (`ldr r1, [r0]` then `adds r1, #4`) and
 * never seeds a base register beside it, which is what the pointer spelling
 * gives. Both forms strength-reduce to the same `adds r1, #4`, so this is a
 * register-allocation readout, not a claim about the original.
 *
 * `cmp r0, #0xfe; bls` is the terminator test spelled `<= 0xfe`. A leaf with no
 * frame -- it ends `bx lr` with no `push` -- so nothing here may spill. */
int CountBuildablePropertiesOfKind(int a1)
{
    struct Unk085766E4 *p;
    int n;

    n = 0;
    p = gUnknown_085766E4;

    while (p->unk00 <= 0xfe)
    {
        if (p->unk02 == a1 && p->unk03 != 0xfe)
            n++;

        p++;
    }

    return n;
}

asm(".global sub_08060ED4\n.thumb_set sub_08060ED4, CountBuildablePropertiesOfKind\n");

void AiCalcBuildPriorities(void)
{
    int i;
    int d;

    for (i = 1; i <= 24; i++)
    {
        d = ((struct Unk60F00Tbl *)gUnknown_085766E0)->rows[i - 1].unk07;

        if (d == 0)
            gUnknown_03004640[i] = 0xff;
        else
            gUnknown_03004640[i] = CountUnitsOfType(i) * 1000 / gUnknown_03004674 * 10 / d;
    }
}

asm(".global sub_08060F00\n.thumb_set sub_08060F00, AiCalcBuildPriorities\n");

/* AiMarkAffordableUnits @ 0x08060F74, 136 bytes.
 *
 * The veto pass over AiCalcBuildPriorities's scores: a type is struck out (0xFF) unless
 * the army still has a use for it -- CountBuildablePropertiesOfKind must find a live request for
 * the type's class, and the type's cost must not exceed the army's funds. Two
 * types, 15 and 16, are struck out unconditionally unless bit 0 of
 * gUnknown_030045C0 is set.
 *
 * `adds r0, r5, #0; subs r0, #0xf; cmp r0, #1; bhi` is NOT a subtract in the
 * source: agbcc folds the plain `i == 15 || i == 16` into one unsigned range
 * test. Written that way it reproduces exactly; do not author the subtract.
 *
 * `lsls r0, r2, #4; subs r0, r0, r2; lsls r0, #2` is (x*16 - x)*4 = x * 60,
 * which is sizeof(struct PlayerStruct) -- the same subscript
 * src/decomp/c_080610D0.c already writes as
 * `gPlayers[gUnknown_030033EC].unk00`, against the same
 * `GetUnitCostWithCoBonus(gUnknown_030033EC, type) * 10`. `bls` is unsigned because that
 * member is, and gUnknown_030033EC is re-read for the subscript rather than
 * CSEd because it is named twice in the source.
 *
 * `movs r6, #0xff` before the loop is LICM hoisting the constant both stores
 * share, not a source variable. Each `strh r6, [r4]` carries the dead `ldrh`
 * of the volatile gUnknown_03004640 documented in include/unknown-globals.h. */
void AiMarkAffordableUnits(void)
{
    int i;

    for (i = 1; i <= 24; i++)
    {
        if (CountBuildablePropertiesOfKind(gUnknown_0857680F[i]) == 0
            || GetUnitCostWithCoBonus(gUnknown_030033EC, i) * 10
                   > gPlayers[gUnknown_030033EC].funds)
            gUnknown_03004640[i] = 0xff;

        if ((i == 15 || i == 16) && (gUnknown_030045C0 & 1) == 0)
            gUnknown_03004640[i] = 0xff;
    }
}

asm(".global sub_08060F74\n.thumb_set sub_08060F74, AiMarkAffordableUnits\n");
