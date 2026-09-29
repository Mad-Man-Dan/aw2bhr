#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063FEC.
 * sub_08063FEC @ 0x08063FEC
 */

/* Builds a rotation about the X axis into the 4x3 20.12 matrix
 * src/decomp/c_08063F98.c documents: cos at m[1][1]/m[2][2], sin at m[2][1]
 * and -sin at m[1][2]. Both callees return `s16`, so agbcc re-narrows each
 * result at the call site and `combine` merges that `asrs #0x10` with the
 * source's `>> 2` into a single `asrs #0x12`.
 *
 * The two halves are NOT spelled the same way, and the ROM is what says so.
 * The cosine is shifted at its DEFINITION and parked in an `s16` local -- the
 * `lsls #0x10; lsrs #0x10` right after the `asrs #0x12` is PROMOTE_MODE
 * zero-extending that local, and the matching `lsls #0x10; asrs #0x10` before
 * the first store is the signed read back. The sine has no such round trip:
 * its `lsls #0x10` sits alone at the call and the `asrs #0x12` does not appear
 * until the first USE, which is what an `int` local shifted at each use looks
 * like. Writing both the same way moves that `asrs` and costs the match. */
struct Mtx43 /* 0x30 */
{
    /* 0x00 */ s32 m[4][3];
};

void BuildRotationMatrixX(struct Mtx43 *mtx, s16 a)
{
    s16 c = CosDegrees(a) >> 2;
    int s = SinDegrees(a);

    mtx->m[0][0] = 0x1000;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = c;
    mtx->m[1][2] = -(s >> 2);
    mtx->m[2][0] = 0;
    mtx->m[2][1] = s >> 2;
    mtx->m[2][2] = c;
}
asm(".global sub_08063FEC\n.thumb_set sub_08063FEC, BuildRotationMatrixX\n");
