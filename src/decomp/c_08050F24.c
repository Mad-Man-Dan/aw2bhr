#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050F24.
 * sub_08050F24 @ 0x08050F24
 */

/* The one lever was the evaluation ORDER inside the gUnknown_08553640
 * subscript: the ROM reads gUnknown_03004580[a ^ 1][2] first, parks it, and
 * only then loads the gUnknown_085D6A48 pool word and walks the row, which
 * fixes the pool order 08553640, 03004580, 085D6A48.
 *
 * Wave 34 recorded that the c_08050B70.c comma trick "does NOT work for the
 * other operand, because operand complexity is decided before the side effect
 * is considered". That is REFUTED. agbcc does expand the more complex operand
 * of the multiply first -- so the fix is not to move the simple operand, it is
 * to put the ASSIGNMENT of the simple operand inside the complex one, ahead of
 * everything else in it, and leave the bare variable as the second operand.
 * The complexity ranking is then untouched and the side effect rides along in
 * the operand that is already going first. */
void SpawnHitEffect(u16 a, u16 b)
{
    u16 *row;
    int c2;
    int n;
    int x;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    n = gUnknown_08552178[a][b] * 7 + 1;

    x = gUnknown_08553640[(c2 = gUnknown_03004580[a ^ 1][2],
                           row = gUnknown_085D6A48[gUnknown_03004580[a ^ 1][1]],
                           row[8]) * c2];

    gUnknown_020298E0[a].unk02[gUnknown_020298E0[a].unk16 - 1] =
        sub_08015410(gUnknown_085535B8[x], 1,
                     gUnknown_02029A00[a ^ 1][0], gUnknown_02029A00[a ^ 1][1], n);
}
asm(".global sub_08050F24\n.thumb_set sub_08050F24, SpawnHitEffect\n");
