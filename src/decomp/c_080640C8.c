#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080640C8.
 * sub_080640C8 @ 0x080640C8
 */

struct Vec3 /* 0x0c */
{
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
};

s32 DotProductVec3(struct Vec3 *a, struct Vec3 *b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z) >> 12;
}
asm(".global sub_080640C8\n.thumb_set sub_080640C8, DotProductVec3\n");
