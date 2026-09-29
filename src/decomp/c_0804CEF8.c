#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804CEF8.
 * sub_0804CEF8 @ 0x0804CEF8
 */

/* Sibling of the matched sub_0804DB14 (src/decomp/c_0804DB14.c), which is the
 * same body with ONE arm where this has two: gUnknown_03004580[a][2] selects
 * between a (0x800, 0x100) triple out of gUnknown_08557978/gUnknown_085533E4
 * and a (0x200, 0x280) one out of gUnknown_085579B4/gUnknown_085533FC. The
 * tail Decompress + sub_08015410 pair is the exemplar's, on the third table
 * gUnknown_085579F0. Zero new declarations -- every global and every prototype
 * was already in the headers, which is what W20-C's data_refs-subset screen
 * predicts and what it is good at.
 *
 * Two spellings are load-bearing and both are already-recorded rules that had
 * to be applied rather than derived:
 *   - `a * 0x2000`, NOT `a << 13`. `a` is a u16 parameter and the sum is
 *     truncated back to u16, so shorten_binary_op folds the MULTIPLY where it
 *     does not fold a shift -- the `g * 0x100` is not `g << 8` rule in
 *     docs/agbcc-codegen.md, here on a parameter rather than a global.
 *   - `u16 n` for the `* 7` count and `int A, B` for the two gUnknown_03004580
 *     reads. The exemplar has `int n` and `u16 A, B` and BOTH are wrong here:
 *     with them the candidate is +2 instructions and spills `off` to the stack
 *     where the ROM keeps it in r7. Widths that are byte-neutral in the
 *     exemplar are not byte-neutral here -- the wave-16 width-sweep warning
 *     read forwards, within one family.
 *
 * Promotion must carry rodata: ["0x08136068"], the -fforce-addr copy of
 * &gUnknown_02029668. Do NOT add `#include "hardware.h"` -- it is not needed
 * and tools/permute.py cannot parse a unit that has it (wave 13). */
void SpawnCruiserFigure(u16 a)
{
    int A, B;
    u16 off;
    u16 n;

    gUnknown_0300453C = a;
    gUnknown_0300451C = gUnknown_08552148[a];
    A = gUnknown_03004580[a][0];
    B = gUnknown_03004580[a][4];
    SpawnWholeFigure(a);

    if (gUnknown_03004580[a][2] <= 1)
    {
        off = a * 0x2000 + 0x800;
        n = gUnknown_08552178[a][2] * 7;
        gUnknown_02029668[a][2] = sub_08015410(gUnknown_085533E4, 1,
                                               gUnknown_08557978[A][2],
                                               gUnknown_08557978[A][1], n);
        CpuFastSet(gUnknown_08557978[gUnknown_08562128[B]][0],
                   (void *)(0x06010000 + off), 0x100);
    }
    else
    {
        off = a * 0x2000 + 0x200;
        n = gUnknown_08552178[a][2] * 7;
        gUnknown_02029668[a][2] = sub_08015410(gUnknown_085533FC, 1,
                                               gUnknown_085579B4[A][2],
                                               gUnknown_085579B4[A][1], n);
        CpuFastSet(gUnknown_085579B4[gUnknown_08562128[B]][0],
                   (void *)(0x06010000 + off), 0x280);
    }

    Decompress(gUnknown_085579F0[A][0], gUnknown_08552FB0[a]);
    n = gUnknown_08552178[a][3] * 7;
    gUnknown_02029668[a][3] = sub_08015410(gUnknown_08553414, 1,
                                           gUnknown_085579F0[A][2],
                                           gUnknown_085579F0[A][1], n);
}
asm(".global sub_0804CEF8\n.thumb_set sub_0804CEF8, SpawnCruiserFigure\n");
