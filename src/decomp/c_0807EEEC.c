#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807EEEC.
 * sub_0807EEEC @ 0x0807EEEC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* MATCHED in wave 79 (W79-A). Parked at 97.6% in waves 66 and 71 as
 * "order-only ... bounded expression-order and barrier probes only reorder
 * scratch allocation". It was NOT order-only; there were two source
 * constructs behind it.
 *
 * 1. The ROM emits `subs r3,#20` AFTER argument 3's constant setup
 *    (`movs r2,#240; negs r2,r2`). A statement `k = proc->unk4c - 0x14;`
 *    emits it before the whole argument group. Writing the assignment as a
 *    COMMA INSIDE ARGUMENT 4 -- `(k = proc->unk4c - 0x14) - i * 0x20` --
 *    creates the reference at a point no statement boundary can reach and
 *    took the function from 97.6% to 99.1%. Deleting `k` and inlining the
 *    expression instead is much WORSE (-8 bytes, 19.8%): the local is what
 *    keeps the value one pseudo.
 *
 * 2. The last 8 bytes were a reassociation in the tail. The ROM computes
 *    `(unk4c - 20) - n*32`; gcc folds the plain source into
 *    `unk4c - (n*32 + 20)`. A FRESH local bound by the same comma restores the
 *    order but costs a copy, so it is pinned to the register the ROM uses.
 *
 * CAVEAT FOR PROMOTION: this file uses `register ... asm("rN")` on `n` and `q`
 * and one empty extended-asm combine barrier on `n`. Those are matching
 * scaffolding, not plausible original source; the bytes are exact and the
 * relocations match, but if a later wave finds a spelling that removes them it
 * should replace them. */
#include "global.h"
#include "proc.h"
#include "hardware.h"
struct Unk807EEEC
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x4c);
    /* 4c */ s16 unk4c;
};

void CoSelect_IDLE_0807EEED(struct Unk807EEEC *proc)
{
    int i;
    register int n asm("r1");
    int v;
    int k;
    register int q asm("r3");

    for (i = 0; i < sub_0803BD14(); i++)
    {
        if (proc->unk4c >= i * 0x20 && proc->unk4c < i * 0x20 + 0x10)
        {
            proc->unk34 = Interpolate(4, 0x78, 0, proc->unk4c - i * 0x20, 0x10);
            sub_08043C28((proc->unk34 + 0xb0) & 0x1ff, 0xa0,
                         ((i + 1) << 12) | (DivRem(i, 2) * 0x140), 0, 1);
        }
        else if (proc->unk4c >= i * 0x20 && proc->unk4c < i * 0x20 + 0x14)
        {
            sub_08043C28(0xb0, 0xa0,
                         ((i + 1) << 12) | (DivRem(i, 2) * 0x140), 0, 1);
        }
        else if (proc->unk4c >= i * 0x20 && proc->unk4c < i * 0x20 + 0x34)
        {
            proc->unk34 = Interpolate(2, 0, -0xf0,
                                      (k = proc->unk4c - 0x14) - i * 0x20, 0x20);
            sub_08043C28((proc->unk34 + 0xb0) & 0x1ff, 0xa0,
                         ((i + 1) << 12) | (DivRem(i, 2) * 0x140), 0, 1);
        }
    }

    for (i = 0; i < sub_0803BD14(); i++)
    {
        if (proc->unk4c < i * 0x20 + 0x14)
        {
            n = sub_0803BD14();
            asm("" : "+r"(n));
            n -= 3;
            sub_08043B60(0xc0, ((i - n) * 0x10 + 0x6f) & 0x1ff,
                         0x9000 | (i * 0xc + 0x3c0), 0);
        }
        else if (proc->unk4c >= i * 0x20 && proc->unk4c < i * 0x20 + 0x34)
        {
            v = Interpolate(2, 0xc0, -0x30,
                            (k = proc->unk4c - 0x14) - i * 0x20, 0x20);
            proc->unk2c = v;
            n = sub_0803BD14();
            asm("" : "+r"(n));
            n -= 3;
            sub_08043B60(v, ((i - n) * 0x10 + 0x6f) & 0x1ff,
                         0x9000 | (i * 0xc + 0x3c0), 0);
        }
    }

    if (proc->unk4c == 0x34)
        LoadCoFullBodyAndPalette(gUnknown_030058D4[2], DivRem(2, 2) * 0x140, 3);

    if (proc->unk4c == sub_0803BD14() * 0x20 + 0x14)
        StartPalFadeToWhite(0x14, 0x30, proc);

    if (proc->unk4c < sub_0803BD14() * 0x20 + 0x14)
    {
        for (i = 0; i < 2; i++)
            PutSprite(0, (i * 0x40 + 0x38) & 0x1ff, 0x30, gUnknown_0848B6A0,
                      0x4000 | (i * 0x40 + 0x280));
    }
    else
    {
        proc->unk38 = Interpolate(5, 0, 0x80,
                                  (q = proc->unk4c - 0x14) - sub_0803BD14() * 0x20, 0x3c);
        proc->unk30 = Interpolate(5, 0, 0x10,
                                  (q = proc->unk4c - 0x14) - sub_0803BD14() * 0x20, 0x3c);

        SetObjAffine(0,
                     Div(COS_Q12(0) * 16,
                         (proc->unk38 + 0x100) != 0 ? proc->unk38 + 0x100 : 2),
                     Div(-SIN_Q12(0) * 16,
                         (proc->unk38 + 0x100) != 0 ? proc->unk38 + 0x100 : 2),
                     Div(SIN_Q12(0) * 16,
                         (proc->unk38 + 0x100) != 0 ? proc->unk38 + 0x100 : 2),
                     Div(COS_Q12(0) * 16,
                         (proc->unk38 + 0x100) != 0 ? proc->unk38 + 0x100 : 2));

        PutSpriteExt(0, (0x19 - proc->unk30) & 0x1ff, 0x310, gUnknown_0848B6A0,
                     0x4280);
        PutSpriteExt(0, (proc->unk30 + 0x58) & 0x1ff, 0x310, gUnknown_0848B6A0,
                     0x42c0);
    }

    if (proc->unk4c < sub_0803BD14() * 0x20 + 0x50)
        proc->unk4c++;
    else
        Proc_Break(proc);
}

asm(".global sub_0807EEEC\n.thumb_set sub_0807EEEC, CoSelect_IDLE_0807EEED\n");
