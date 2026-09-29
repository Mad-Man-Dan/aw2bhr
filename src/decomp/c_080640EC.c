#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080640EC.
 * sub_080640EC @ 0x080640EC
 */

/* Part of the 20.12 fixed-point matrix library at 0x08063E28-0x080640EC; see
 * c_08063F98.c for the layout and its proof. This is the sibling of the dot
 * product at DotProductVec3, which is the function immediately before it.
 */
struct Vec3 /* 0x0c */
{
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
};

/* Cross product, renormalised from 24.24 back to 20.12 with `>> 12`. */
void CrossProductVec3(struct Vec3 *a, struct Vec3 *b, struct Vec3 *dst)
{
    dst->x = (a->y * b->z - a->z * b->y) >> 12;
    dst->y = (a->z * b->x - a->x * b->z) >> 12;
    dst->z = (a->x * b->y - a->y * b->x) >> 12;
}
asm(".global sub_080640EC\n.thumb_set sub_080640EC, CrossProductVec3\n");
