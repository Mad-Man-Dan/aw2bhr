#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080791B0.
 * sub_080791B0 @ 0x080791B0
 */

/* MATCHED (wave 55, W55-D). 668 bytes, byte-for-byte identical.
 *
 * The 7-byte residual W55-B parked at (99.0%) was the two source givs of the
 * `+ 0x3000` / `+ 0x3400` block getting r5 and r6 the other way round from the
 * ROM. The handback diagnosis named the mechanism correctly (two pointers in
 * the opposite order) but the prescribed fix -- "swap the declaration /
 * initialisation order of those two pointer locals" -- could not be applied,
 * because there are no pointer locals in this function: every pointer here is
 * a giv that strength_reduce created, and the two `+= 128` statements in the
 * tail are giv updates, not source statements.
 *
 * The actual lever is the SPELLING of the source address expression:
 *
 *   gUnknown_0200FC50 + 0x3000 + i * 0x80        -> src1 in r6  (7 bytes off)
 *   gUnknown_0200FC50 + (0x180 + i * 4) * 0x20   -> src1 in r5  (match)
 *
 * Both fold to the identical instruction stream -- same pool words, same
 * order, same giv update order. They differ only in the order the two givs
 * enter the RTL and therefore in the allocno numbers that break the r5/r6
 * priority tie (the wave-17 allocno_compare rule). The winning spelling is the
 * one the four neighbouring blocks already use, and that is the point: this
 * block indexes the same table in the same units as blocks 6-8, so the
 * original source spelled it the same way. `0x3000 == 0x180 * 0x20` and
 * `0x3400 == 0x1A0 * 0x20`.
 *
 * RULE FOR THE DOC: when one block of a repeated series is spelled in
 * different UNITS from its siblings (raw byte offset against index * stride)
 * and everything but its register allocation matches, respell it in the
 * siblings' units before touching anything else. Same instructions, different
 * RTL creation order, different tie-break.
 *
 * `relocs: name different symbols that resolve to the same address` at +0x260
 * (gUnknown_02013750 against gUnknown_0200FC50 + 0x3B00) is the documented
 * false mismatch and is not a difference.
 */

struct Unk80791B0
{
    /* 0x00 */ u8 filler_00[0x52];
    /* 0x52 */ u16 unk52;
};

void ResultsScreen_LoadRankGraphics(struct Unk80791B0 *proc)
{
    int i;
    int n;
    int t;
    u16 *q;

    EndPalFade();
    ApplyPaletteExt(gUnknown_0822AA80, 0x280, 0xc0);

    n = gPlayers[GetResultsArmy()].rank;
    q = &proc->unk52;
    *q = 5 - n;
    if (*q > 3)
        *q = 3;

    for (i = 0; i < 8; i++)
    {
        t = i * 0x20 + 0x200;
        CpuFastSet(gUnknown_0200FC50 + (proc->unk52 * 8 + t) * 0x20,
                   (void *)(0x06014800 + i * 0x100), 0x40);
    }

    for (i = 0; i < 10; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x100 + i * 2) * 0x20,
                   (void *)(0x06015000 + i * 0x80), 0x10);
        CpuFastSet(gUnknown_0200FC50 + (0x120 + i * 2) * 0x20,
                   (void *)(0x06015040 + i * 0x80), 0x10);
    }

    for (i = 0; i < 6; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x114 + i * 2) * 0x20,
                   (void *)(0x06015500 + i * 0x80), 0x10);
        CpuFastSet(gUnknown_0200FC50 + (0x134 + i * 2) * 0x20,
                   (void *)(0x06015540 + i * 0x80), 0x10);
    }

    for (i = 0; i < 4; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x140 + i * 2) * 0x20,
                   (void *)(0x06015800 + i * 0x80), 0x10);
        CpuFastSet(gUnknown_0200FC50 + (0x160 + i * 2) * 0x20,
                   (void *)(0x06015840 + i * 0x80), 0x10);
    }

    CpuFastSet(gUnknown_0200FC50 + 0x3B00, (void *)0x06015A00, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x3F00, (void *)0x06015A80, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x2B80, (void *)0x06015F80, 0x20);

    for (i = 0; i < 2; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x180 + i * 4) * 0x20,
                   (void *)(0x06016000 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (0x1A0 + i * 4) * 0x20,
                   (void *)(0x06016080 + i * 0x100), 0x20);
    }

    for (i = 0; i < 2; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x188 + i * 4) * 0x20,
                   (void *)(0x06016200 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (0x1A8 + i * 4) * 0x20,
                   (void *)(0x06016280 + i * 0x100), 0x20);
    }

    for (i = 0; i < 2; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x190 + i * 4) * 0x20,
                   (void *)(0x06016400 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (0x1B0 + i * 4) * 0x20,
                   (void *)(0x06016480 + i * 0x100), 0x20);
    }

    for (i = 0; i < 2; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (0x1C0 + i * 4) * 0x20,
                   (void *)(0x06016600 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (0x1E0 + i * 4) * 0x20,
                   (void *)(0x06016680 + i * 0x100), 0x20);
    }
}
asm(".global sub_080791B0\n.thumb_set sub_080791B0, ResultsScreen_LoadRankGraphics\n");
