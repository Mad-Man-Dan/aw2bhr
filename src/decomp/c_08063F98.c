#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063F98.
 * sub_08063F98 @ 0x08063F98, sub_08063FB8 @ 0x08063FB8
 */

/* A 4x3 fixed-point matrix: three rows of basis vectors plus a translation
 * row, elements in 20.12 (0x1000 == 1.0). The layout is proved by the matrix
 * multiply at MultiplyMatrix43, which reserves exactly 0x30 bytes of stack for one
 * of these and indexes the right-hand operand's columns at a stride of 0x0c.
 */
struct Mtx43 /* 0x30 */
{
    /* 0x00 */ s32 m[4][3];
};

/* Load the identity. 0x1000 lands at +0x00, +0x10 and +0x20, which are exactly
 * m[0][0], m[1][1] and m[2][2] under the 0x0c row stride -- the cleanest
 * confirmation of the layout available.
 */
void LoadIdentityMatrix43(struct Mtx43 *mtx)
{
    mtx->m[0][0] = 0x1000;
    mtx->m[0][1] = 0;
    mtx->m[0][2] = 0;
    mtx->m[1][0] = 0;
    mtx->m[1][1] = 0x1000;
    mtx->m[1][2] = 0;
    mtx->m[2][0] = 0;
    mtx->m[2][1] = 0;
    mtx->m[2][2] = 0x1000;
    mtx->m[3][0] = 0;
    mtx->m[3][1] = 0;
    mtx->m[3][2] = 0;
}
asm(".global sub_08063F98\n.thumb_set sub_08063F98, LoadIdentityMatrix43\n");

/* Copy a matrix. This must be written out element by element: `*dst = *src` on
 * a 0x30-byte struct compiles to four `ldmia`/`stmia` pairs through r2-r4 and
 * a `push {r4, lr}` frame, which is not what the ROM has. The interleaved
 * ldr/str shape is the element-wise source, exactly as the "8-byte struct
 * assignment" note in docs/agbcc-codegen.md predicts for the small case.
 */
void CopyMatrix43(struct Mtx43 *src, struct Mtx43 *dst)
{
    dst->m[0][0] = src->m[0][0];
    dst->m[0][1] = src->m[0][1];
    dst->m[0][2] = src->m[0][2];
    dst->m[1][0] = src->m[1][0];
    dst->m[1][1] = src->m[1][1];
    dst->m[1][2] = src->m[1][2];
    dst->m[2][0] = src->m[2][0];
    dst->m[2][1] = src->m[2][1];
    dst->m[2][2] = src->m[2][2];
    dst->m[3][0] = src->m[3][0];
    dst->m[3][1] = src->m[3][1];
    dst->m[3][2] = src->m[3][2];
}
asm(".global sub_08063FB8\n.thumb_set sub_08063FB8, CopyMatrix43\n");
