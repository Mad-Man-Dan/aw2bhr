#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061928.
 * sub_08061928 @ 0x08061928
 */

/* A 0x130-byte gUnknown_085771C4 record built from two others: the 0x10-byte
 * header is copied verbatim from the record selected DIRECTLY by a2, and the
 * 24 following 0xc-byte rows are the byte-wise sum of that record and the one
 * selected through gUnknown_0857690C[a3][gPlayers[a4].unk1d]. Both
 * strides are the `((x*4+x)*4-x)*16` synthesis of *0x130 that AiLoadPersonality
 * already uses, so indexing gUnknown_085771C4[] directly reproduces them.
 *
 * The 0x10 unrolled byte pairs and the 0xc-wide loop body are SOURCE-LEVEL,
 * not unrolling: agbcc at this project's -O2 does not unroll (probed -- a
 * `for (i = 0; i < 0x10; i++)` copy and a nested `for (j = 0; j < 12; j++)`
 * both stay rolled, and give strength-reduced givs instead). Nor is the header
 * a struct assignment: ARM's 32-bit STRUCTURE_SIZE_BOUNDARY aligns any struct
 * to 4, so `d->hdr = s->hdr` on a 0x10-byte struct emits
 * `ldmia/stmia {r4,r6,r7}` plus an `ldr/str`, and the whole-record `*d = *s`
 * emits a `bl memcpy`. Only byte-typed statements give the ROM's `ldrb`/`strb`
 * pairs. That also settles the register file: the ldmia form needs r7 and
 * pushes {r4,r5,r6,r7}, where the ROM pushes {r4,r5,r6} only.
 *
 * Operand order is read off which side loads into r0 first, and it is the
 * RIGHT one: the ROM loads the table-selected record first, so it is written
 * second.
 */

struct Row8061928 { u8 b[0xc]; };
/* The 0x130 record as this function views it. gUnknown_085771C4's shared type
 * stays `u8 filler_00[0x130]` -- this is a cast view of the same bytes, not a
 * reshape of it. */
struct Unk8061928
{
    /* 0x00 */ u8 unk00[0x10];
    /* 0x10 */ struct Row8061928 unk10[24];
};

void sub_08061928(struct Unk085771C4 *dst, u8 a2, u8 a3, u16 a4)
{
    struct Unk8061928 *d;
    const struct Unk8061928 *s1;
    const struct Unk8061928 *s2;
    int i;

    d = (struct Unk8061928 *)dst;
    s1 = (const struct Unk8061928 *)&gUnknown_085771C4[a2];
    s2 = (const struct Unk8061928 *)&gUnknown_085771C4[gUnknown_0857690C[a3][gPlayers[a4].co]];

    d->unk00[0] = s1->unk00[0];
    d->unk00[1] = s1->unk00[1];
    d->unk00[2] = s1->unk00[2];
    d->unk00[3] = s1->unk00[3];
    d->unk00[4] = s1->unk00[4];
    d->unk00[5] = s1->unk00[5];
    d->unk00[6] = s1->unk00[6];
    d->unk00[7] = s1->unk00[7];
    d->unk00[8] = s1->unk00[8];
    d->unk00[9] = s1->unk00[9];
    d->unk00[10] = s1->unk00[10];
    d->unk00[11] = s1->unk00[11];
    d->unk00[12] = s1->unk00[12];
    d->unk00[13] = s1->unk00[13];
    d->unk00[14] = s1->unk00[14];
    d->unk00[15] = s1->unk00[15];

    for (i = 0; i < 24; i++)
    {
        d->unk10[i].b[0] = s1->unk10[i].b[0] + s2->unk10[i].b[0];
        d->unk10[i].b[1] = s1->unk10[i].b[1] + s2->unk10[i].b[1];
        d->unk10[i].b[2] = s1->unk10[i].b[2] + s2->unk10[i].b[2];
        d->unk10[i].b[3] = s1->unk10[i].b[3] + s2->unk10[i].b[3];
        d->unk10[i].b[4] = s1->unk10[i].b[4] + s2->unk10[i].b[4];
        d->unk10[i].b[5] = s1->unk10[i].b[5] + s2->unk10[i].b[5];
        d->unk10[i].b[6] = s1->unk10[i].b[6] + s2->unk10[i].b[6];
        d->unk10[i].b[7] = s1->unk10[i].b[7] + s2->unk10[i].b[7];
        d->unk10[i].b[8] = s1->unk10[i].b[8] + s2->unk10[i].b[8];
        d->unk10[i].b[9] = s1->unk10[i].b[9] + s2->unk10[i].b[9];
        d->unk10[i].b[10] = s1->unk10[i].b[10] + s2->unk10[i].b[10];
        d->unk10[i].b[11] = s1->unk10[i].b[11] + s2->unk10[i].b[11];
    }
}
