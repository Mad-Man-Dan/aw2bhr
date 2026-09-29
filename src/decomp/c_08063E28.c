#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063E28.
 * sub_08063E28 @ 0x08063E28
 */

/* Multiply two 4x3 fixed-point matrices, 20.12, into a third, aliasing-safe.
 *
 * Twelve near-identical 13-instruction blocks with a two-arm prologue and a
 * one-arm epilogue -- 368 bytes and the plainest large function in the tree.
 * It matched on the first probe with no register fighting at all, which is the
 * useful data point: size is not what makes a function hard.
 *
 * The alias guard is what the two branches are. `out` is a pointer local set to
 * either the destination or a stack scratch, and the copy at the end is
 * conditional on which -- `cmp r5, sp` is literally `out == &tmp`. Do not try
 * to express it as an if/else around the whole body; the twelve stores exist
 * once and go through `out`.
 *
 * The `mul` operand order in the emitted assembly reads backwards from the
 * ROM's disassembly (`mul rD, rD, rS` here vs `muls rD, rS, rD` there) and that
 * is only the two spellings of one THUMB encoding -- MUL has a single
 * destination-tied form. Do not spend an attempt on it.
 */

struct Mtx43 /* 0x30 */
{
    /* 0x00 */ s32 m[4][3];
};
/* Declared here rather than in unknown-functions.h because its signature needs
 * struct Mtx43, which is local to this pair of files. See
 * work/CopyMatrix43/CopyMatrix43.c, which carries the same definition. */
void CopyMatrix43(struct Mtx43 *src, struct Mtx43 *dst);

void MultiplyMatrix43(struct Mtx43 *a, struct Mtx43 *b, struct Mtx43 *dst)
{
    struct Mtx43 tmp;
    struct Mtx43 *out;

    if (a == dst || b == dst)
        out = &tmp;
    else
        out = dst;

    out->m[0][0] = (a->m[0][0] * b->m[0][0] + a->m[0][1] * b->m[1][0] + a->m[0][2] * b->m[2][0]) >> 12;
    out->m[0][1] = (a->m[0][0] * b->m[0][1] + a->m[0][1] * b->m[1][1] + a->m[0][2] * b->m[2][1]) >> 12;
    out->m[0][2] = (a->m[0][0] * b->m[0][2] + a->m[0][1] * b->m[1][2] + a->m[0][2] * b->m[2][2]) >> 12;
    out->m[1][0] = (a->m[1][0] * b->m[0][0] + a->m[1][1] * b->m[1][0] + a->m[1][2] * b->m[2][0]) >> 12;
    out->m[1][1] = (a->m[1][0] * b->m[0][1] + a->m[1][1] * b->m[1][1] + a->m[1][2] * b->m[2][1]) >> 12;
    out->m[1][2] = (a->m[1][0] * b->m[0][2] + a->m[1][1] * b->m[1][2] + a->m[1][2] * b->m[2][2]) >> 12;
    out->m[2][0] = (a->m[2][0] * b->m[0][0] + a->m[2][1] * b->m[1][0] + a->m[2][2] * b->m[2][0]) >> 12;
    out->m[2][1] = (a->m[2][0] * b->m[0][1] + a->m[2][1] * b->m[1][1] + a->m[2][2] * b->m[2][1]) >> 12;
    out->m[2][2] = (a->m[2][0] * b->m[0][2] + a->m[2][1] * b->m[1][2] + a->m[2][2] * b->m[2][2]) >> 12;
    out->m[3][0] = ((a->m[0][0] * b->m[3][0] + a->m[1][0] * b->m[3][1] + a->m[2][0] * b->m[3][2]) >> 12) + a->m[3][0];
    out->m[3][1] = ((a->m[0][1] * b->m[3][0] + a->m[1][1] * b->m[3][1] + a->m[2][1] * b->m[3][2]) >> 12) + a->m[3][1];
    out->m[3][2] = ((a->m[0][2] * b->m[3][0] + a->m[1][2] * b->m[3][1] + a->m[2][2] * b->m[3][2]) >> 12) + a->m[3][2];

    if (out == &tmp)
        CopyMatrix43(&tmp, dst);
}
asm(".global sub_08063E28\n.thumb_set sub_08063E28, MultiplyMatrix43\n");
