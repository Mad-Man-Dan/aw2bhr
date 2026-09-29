#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063DDC.
 * sub_08063DDC @ 0x08063DDC
 */

/* Part of the 20.12 fixed-point matrix library at 0x08063E28-0x080640EC; see
 * c_08063F98.c for the layout and its proof.
 */
struct Mtx43 /* 0x30 */
{
    /* 0x00 */ s32 m[4][3];
};
struct Vec3 /* 0x0c */
{
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
};

/* Transform a vector by the rotation part of a 4x3 matrix: dst = v * m, using
 * rows 0-2 only. Row 3, the translation, is untouched -- which is what makes
 * this the direction-vector transform rather than the point transform.
 *
 * v's three components are loaded once each into r6/r5/r4 and survive all three
 * output stores: the stores are SImode, and a word store does not invalidate a
 * cached load here. No local is needed to get that.
 */
void TransformVec3ByRotation(struct Vec3 *v, struct Mtx43 *mtx, struct Vec3 *dst)
{
    dst->x = (v->x * mtx->m[0][0] + v->y * mtx->m[1][0] + v->z * mtx->m[2][0]) >> 12;
    dst->y = (v->x * mtx->m[0][1] + v->y * mtx->m[1][1] + v->z * mtx->m[2][1]) >> 12;
    dst->z = (v->x * mtx->m[0][2] + v->y * mtx->m[1][2] + v->z * mtx->m[2][2]) >> 12;
}
asm(".global sub_08063DDC\n.thumb_set sub_08063DDC, TransformVec3ByRotation\n");
