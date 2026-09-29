#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B930.
 * sub_0803B930 @ 0x0803B930
 */

/* MATCHED byte-for-byte (wave 37, W37-Q2).
 *
 * "Seed the per-slot arrays for `a` players, then hand over to sub_0803B8C4."
 * Both loops count with a u8: `adds r0,rI,#1; lsls #0x18; lsrs rI,#0x18` is
 * the u8 wraparound and both exit tests are `bls`, i.e. unsigned.
 *
 * gUnknown_0849EA78 is a ROM byte table read at [i - 1], and the -1 is NOT
 * folded into the base (`subs r1,r2,#1; add r1,ip; ldrb`) because that base is
 * a pool word, whereas the same -1 IS free on a struct member (an `adds #k`
 * chain). That asymmetry is what tells the two index origins apart, and it is
 * why the table index is spelled i-1 while the struct index is spelled i.
 *
 * THE LOAD-BEARING SPELLING is `unk38[i]` in BOTH arms of the `if`. Writing
 * the then-arm as the constant `unk38[1]` stores the same value -- agbcc knows
 * i == 1 there and reuses r2 rather than materialising a constant either way --
 * but it is not the same code. With `unk38[1]` agbcc folds the offset into the
 * literal pool (`.word gPlaySt+0x38`) and derives 0x3d and 0x39 from
 * that with one `adds #imm3` each; with `unk38[i]` in both arms it emits the
 * UNBIASED `.word gPlaySt` and pays `adds rX,r0,#0; adds rX,#K` for
 * all three offsets. The ROM has the unbiased form, which is 8 bytes longer
 * and takes four callee-saved registers instead of three. See the "Loops"
 * chapter of docs/agbcc-codegen.md.
 *
 * The three hoisted addresses are LICM output and not source statements: they
 * sit AFTER the loop-entry guard `cmp r2,r3; bhi`, not before it.
 */

void StartNewVersus(u8 a)
{
    u8 i;

    gUnknown_0849ECDC->unk00 = a;

    for (i = 1; i <= 4; i++)
        gUnknown_03003FF8[i] = 0;

    for (i = 1; i <= a; i++)
    {
        gPlaySt.co[i] = gUnknown_0849EA78[i - 1];
        if (i == 1)
            gPlaySt.aiControlled[i] = 1;
        else
            gPlaySt.aiControlled[i] = 2;
    }

    sub_0803B8C4();
}
asm(".global sub_0803B930\n.thumb_set sub_0803B930, StartNewVersus\n");
