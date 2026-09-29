#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063CCC.
 * sub_08063CCC @ 0x08063CCC
 */

#include "hardware.h"

/* The rotating-sprite idiom of its matched neighbour sub_08063BE0
 * (src/decomp/c_08063BE0.c): four Div calls feeding SetObjAffine, every
 * divisor guarded by the same `!= 0 ? : 2` ternary, then one PutOamHi.
 *
 * The two `lsls #0x14 / lsrs #0x10` and `lsls #0x12 / lsrs #0x10` pairs on the
 * merged path are not a mask plus a shift: read as `(u32)x << a >> b` they are
 * `(u16)(a5 * 0x10)` and `(u16)(a5 * 4)`. The truncation belongs to the u16
 * LOCAL, not to PutOamHi's parameter -- the middle arm calls PutOamHi
 * directly with `a5 * 4` and emits a bare `lsls r3, r7, #2` with no truncation,
 * which it could not do if the fourth parameter were narrow.
 *
 * The three computed values are assigned BACK INTO THE PARAMETERS rather than
 * into fresh locals, and that is the whole difference between 64.0% and a
 * match: with fresh locals the frame is the right size and the instruction
 * stream is otherwise identical, but x, y and the divisor lose r8/r9/sl to the
 * 0x200 constant and to `w`, which costs one extra `mov r8, r1` (absorbed by
 * the alignment slot ahead of the first pool word, so the SIZE stays exact).
 * Reusing the parameters gives x, y and v the entry pseudos, which are the
 * earliest allocnos in the function, and t/w/the first Div result spill
 * instead -- exactly the ROM's assignment.
 */
void DrawZoomedSpriteFromCentre(int a1, int a2, int a3, int a4, int a5)
{
    void *t;
    u16 w;

    a1 = a1 * 0x200 / (0x200 - a3) + 0x78;
    a2 = a2 * 0x200 / (0x200 - a3) + 0x50;
    a3 = a3 * 3 + 0x100;

    if (a3 > 0x200)
    {
        t = gUnknown_0858081C[a4];
        w = a5 * 0x10;
    }
    else if (a3 == 0x100)
    {
        PutOamHi(a1, a2, gUnknown_085806EA, a5 * 4);
        return;
    }
    else
    {
        t = gUnknown_0858085C[a4];
        w = a5 * 4;
    }

    SetObjAffine(a4,
                 Div(COS_Q12(0) * 16, a3 != 0 ? a3 : 2),
                 Div(-SIN_Q12(0) * 16, a3 != 0 ? a3 : 2),
                 Div(SIN_Q12(0) * 16, a3 != 0 ? a3 : 2),
                 Div(COS_Q12(0) * 16, a3 != 0 ? a3 : 2));

    PutOamHi(a1, a2, t, w);
}
asm(".global sub_08063CCC\n.thumb_set sub_08063CCC, DrawZoomedSpriteFromCentre\n");
