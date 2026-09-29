#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807AE94.
 * sub_0807AE94 @ 0x0807AE94, sub_0807B148 @ 0x0807B148
 */

#include "proc.h"
#include "hardware.h"
/* Closed in wave 54 (W54-F) from W54-A's 98.3% size-exact draft. The residual
 * was NOT the pointer-binding class: it was the W54-B shared-local live web.
 * `t` and `v` were ONE local each across all three loops, which gave each
 * pseudo a three-range live web and cost the allocator the r0/r1 assignment at
 * every site (12 bytes, exactly the twelve W54-B measured on CoDesignEditor_DrawEnterPicker).
 * The fix is that chapter's, verbatim: an inline ASSIGNMENT expression inside
 * the argument -- which is both the fold barrier for `A - (B - C)` / `X + (Y -
 * C)` and evaluated where it is written -- with a FRESH name per site
 * (t0/t1/t2). `v` then needs no local at all.
 *
 * PROMOTION NOTE: the pool word at 0x081D931C holds &gUnknown_0202FDEC.
 * This match needs "rodata": ["0x081D931C"] in data/promoted.json.
 */

struct Unk0807AE94Proc
{
    /* 0x00 */ u8 unk00[0x34];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 unk38[4];
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 unk40[0xc];
    /* 0x4c */ s16 unk4c;
};
/* VERIFIED MATCH (wave 54, W54-A) -- try_match reports matched, 320/320 bytes.
 *
 * PROMOTION NOTE: this function needs its -fforce-addr pool word placed.
 * data/promoted.json entry must carry:  "rodata": ["0x081D9320"]
 * The word at 0x081D9320 holds 0x0202FDEC, i.e. &gUnknown_0202FDEC. It is NOT
 * a global -- see the 081D93xx block in include/unknown-globals.h.
 *
 * This function is the tree's cleanest proof of that: it reads
 * gUnknown_0202FDEC.unk08 in BOTH arms of the `if`, and agbcc compiles the
 * many-reads arm through the .rodata pool word (ldr; ldr; ldrb) and the
 * one-read arm as a direct `ldr rN, =gUnknown_0202FDEC` (ldr; ldrb) -- from
 * one honest spelling of one global.
 *
 * The `|` operand order in the third argument is load-bearing and is the whole
 * difference between this and a near-miss: writing the multiply term on the
 * LEFT and the shift term on the RIGHT leaves `(i + K) << 12` unreduced, which
 * is what the ROM has. Swapping them makes agbcc strength-reduce it into a
 * second accumulator, which costs a callee-saved register and shifts the
 * allocation of every other value in the loop.
 */

struct Unk0807B148Proc
{
    /* 0x00 */ u8 unk00[0x3c];
    /* 0x3c */ int unk3c;
};

void MatchSummary_SlideIn_Loop(struct Unk0807AE94Proc * proc)
{
    int i;
    int t0;
    int t1;
    int t2;
    struct Unk0807AE94Proc * child;

    if (proc->unk4c <= 0x20)
        proc->unk34 = Interpolate(0, 0, 0xf0, proc->unk4c, 0x20);
    else if (proc->unk4c <= 0x24)
        proc->unk34 = Interpolate(0, 0xf0, 0xe0, proc->unk4c - 0x20, 4);
    else if (proc->unk4c <= 0x28)
        proc->unk34 = Interpolate(0, 0xe0, 0xf0, proc->unk4c - 0x24, 4);
    else if (proc->unk4c <= 0x2a)
        proc->unk34 = Interpolate(0, 0xf0, 0xe8, proc->unk4c - 0x28, 2);
    else if (proc->unk4c <= 0x2c)
        proc->unk34 = Interpolate(0, 0xe8, 0xf0, proc->unk4c - 0x2a, 2);
    else if (proc->unk4c <= 0x2d)
        proc->unk34 = Interpolate(0, 0xf0, 0xec, proc->unk4c - 0x2c, 1);
    else if (proc->unk4c <= 0x2e)
        proc->unk34 = Interpolate(0, 0xec, 0xf0, proc->unk4c - 0x2d, 1);
    else
    {
        proc->unk4c = 0;

        if (proc->unk3c == 0)
            Decompress(gUnknown_0822B944, gBG1TilemapBuffer);
        else
            Decompress(gUnknown_0822BCF0, gBG1TilemapBuffer);

        BG_EnableSyncBG1();

        child = Proc_Start(gUnknown_08616034, proc);
        child->unk3c = proc->unk3c;

        gUnknown_030030B4.bits.tm_block = 15;
        gUnknown_030030B4.bits.chr_block = 2;

        ClearBg2Tilemap();
        BG_EnableSyncBG2();
        LoadBg1WindowFrame(GetResultsArmy());
        Proc_Break(proc);
    }

    if (proc->unk4c == 0x20)
        PlayMusicOrSfx2(0x78);

    gUnknown_03002B34 = 0xf0 - proc->unk34;
    gUnknown_030030A0 = proc->unk34 - 0xf0;
    proc->unk4c++;

    if (proc->unk3c == 0)
    {
        for (i = gUnknown_0202FDEC.unk08 - 1; i >= 0; i--)
            sub_08043FD8((gUnknown_086160A4[gUnknown_0202FDEC.unk08 - 1]
                          + i * gUnknown_086160C8[gUnknown_0202FDEC.unk08 - 1]
                          + (t0 = proc->unk34 - 0xf0)) & 0x1ff,
                         0x30,
                         (i * 0x24 + 0x100) | ((i + 9) << 12),
                         0);

        for (i = 0; i < gUnknown_0202FDEC.unk09; i++)
            sub_08043FD8((((gUnknown_086160AC[gUnknown_0202FDEC.unk09 - 1]
                            - gUnknown_086160CE[gUnknown_0202FDEC.unk09 - 1] * (2 - i))
                           - (t1 = proc->unk34 - 0xf0)) & 0x1ff) | 0x1000,
                         0xa0,
                         (i * 0x24 + 0x16c) | ((i + 0xc) << 12),
                         0);
    }
    else
    {
        for (i = 0; i < gUnknown_0202FDEC.unk08; i++)
            sub_08043FD8((((gUnknown_086160B8[gUnknown_0202FDEC.unk08 - 1]
                            - gUnknown_086160D4[gUnknown_0202FDEC.unk08 - 1] * (3 - i))
                           - (t2 = proc->unk34 - 0xf0)) & 0x1ff) | 0x1000,
                         0xa0,
                         (i * 0x24 + 0x100) | ((i + 9) << 12),
                         0);
    }
}
asm(".global sub_0807AE94\n.thumb_set sub_0807AE94, MatchSummary_SlideIn_Loop\n");

void MatchSummaryFaces_Loop(struct Unk0807B148Proc * proc)
{
    int i;

    if (proc->unk3c == 0)
    {
        for (i = gUnknown_0202FDEC.unk08 - 1; i >= 0; i--)
            sub_08043FD8((gUnknown_086160DC[gUnknown_0202FDEC.unk08 - 1]
                          + i * gUnknown_08616100[gUnknown_0202FDEC.unk08 - 1]) & 0x1ff,
                         0x30,
                         (i * 0x24 + 0x100) | ((i + 9) << 12),
                         0);

        for (i = 0; i < gUnknown_0202FDEC.unk09; i++)
            sub_08043FD8(((gUnknown_086160E4[gUnknown_0202FDEC.unk09 - 1]
                           - gUnknown_08616106[gUnknown_0202FDEC.unk09 - 1] * (2 - i)) & 0x1ff) | 0x1000,
                         0xa0,
                         (i * 0x24 + 0x16c) | ((i + 0xc) << 12),
                         0);
    }
    else
    {
        for (i = 0; i < gUnknown_0202FDEC.unk08; i++)
            sub_08043FD8(((gUnknown_086160F0[gUnknown_0202FDEC.unk08 - 1]
                           - gUnknown_0861610C[gUnknown_0202FDEC.unk08 - 1] * (3 - i)) & 0x1ff) | 0x1000,
                         0xa0,
                         (i * 0x24 + 0x100) | ((i + 9) << 12),
                         0);
    }
}
asm(".global sub_0807B148\n.thumb_set sub_0807B148, MatchSummaryFaces_Loop\n");
