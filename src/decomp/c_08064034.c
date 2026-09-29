#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064034.
 * sub_08064034 @ 0x08064034, sub_0806407C @ 0x0806407C
 */

/* The 4x3 20.12 matrix src/decomp/c_08063F98.c documents as `struct Mtx43`,
 * re-declared under a per-function tag so that promote.py can merge this draft
 * into a unit that already defines that one. */
struct Unk64034Mtx /* 0x30 */
{
    /* 0x00 */ s32 m[4][3];
};
struct Unk6407CMtx /* 0x30 */
{
    /* 0x00 */ s32 m[4][3];
};

/* A rotation about the Y axis: cos on the two diagonal entries, sin at
 * m[2][0] and -sin at m[0][2], with m[1][1] the 20.12 one (0x1000).
 * CosDegrees is cos(deg) and SinDegrees sin(deg); the `>> 2` scales the
 * table's 14-bit result down to 20.12.
 *
 * `c` and `s` are BOTH `s16`, and the asymmetry between them is agbcc's, not
 * the source's: `c` is live across the second `bl`, so its narrow local is
 * stored zero-extended (`lsls #0x10; lsrs #0x10`) and re-signed at the use
 * (`lsls #0x10; asrs #0x10`), while `s` is defined and consumed with no call
 * in between and combine proves the round trip a no-op after `asrs #0x12` and
 * deletes it. Reproduced exactly by declaring both `s16`. */
void BuildRotationMatrixY(struct Unk64034Mtx *mtx, s16 angle)
{
    s16 c = CosDegrees(angle) >> 2;
    s16 s = SinDegrees(angle) >> 2;

    mtx->m[0][0] = c;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = -s;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = 0x1000;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = s;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = c;
}
asm(".global sub_08064034\n.thumb_set sub_08064034, BuildRotationMatrixY\n");

/* BuildRotationMatrixY's Z-axis twin: the same cos/sin pair, placed at m[0][0]/m[1][1]
 * with -sin at m[0][1] and +sin at m[1][0], and the 0x1000 moved to m[2][2].
 * The row of stores is the whole difference between the two functions. */
void BuildRotationMatrixZ(struct Unk6407CMtx *mtx, s16 angle)
{
    s16 c = CosDegrees(angle) >> 2;
    s16 s = SinDegrees(angle) >> 2;

    mtx->m[0][0] = c;
    mtx->m[0][1] = -s;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = s;
    mtx->m[1][1] = c;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = 0x1000;
}
asm(".global sub_0806407C\n.thumb_set sub_0806407C, BuildRotationMatrixZ\n");
