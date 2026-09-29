#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B360.
 * sub_0807B360 @ 0x0807B360
 */

/* VERIFIED MATCH (wave 54, W54-A) -- try_match reports matched, 444/444 bytes,
 * "MATCH -- byte-for-byte identical to the original".
 *
 * The one reloc note is a false mismatch of the kind the brief describes: at
 * +0x180 the ROM names gUnknown_02013750 and this candidate names
 * gUnknown_0200FC50 with addend 0x3b00. Same address. gUnknown_02013750 is not
 * a separate object -- it is 0x3b00 into gUnknown_0200FC50, and the splitter
 * invented a symbol there because agbcc folded the offset into the address
 * constant. Do NOT declare gUnknown_02013750.
 *
 * LOAD-BEARING SPELLING, and it was the whole diff: the three copy loops must
 * scale `i`, not the symbol -- `gUnknown_0200FC50 + (i + 0x80) * 0x40`, NOT
 * `gUnknown_0200FC50 + 0x2000 + i * 0x40`. The second form lets agbcc fold the
 * constant into a NEW address constant (one pool word per loop), where the ROM
 * rematerialises the bare symbol and adds an offset built as `movs #K; lsls
 * #6`. The tell is decisive and readable straight off the ROM: every one of
 * those six constants is `movs #K; lsls #6` where K (0x80, 0x90, 0x8a, 0x9a,
 * 0xa0, 0xb0) is exactly the addend the `(i + K)` form needs.
 *
 * The straight-line tail is the opposite case -- there agbcc DOES fold, so the
 * eight tail copies are written as plain `gUnknown_0200FC50 + 0x3b00` etc.
 *
 * Fixing the loops also fixed the tail: before the fix, the literal 0 stored to
 * unk4c was materialised at its store, where the ROM holds it in a callee-saved
 * register across all eight tail calls. No tail edit was needed once the loop
 * address form was right.
 */

struct Unk0807B360Proc
{
    /* 0x00 */ u8 unk00[0x4c];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u8 unk4e[0xa];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

void MatchSummaryPanel_LoadGraphics(struct Unk0807B360Proc * proc)
{
    int i;

    Decompress(gUnknown_08227F3C, gUnknown_0200FC50);

    for (i = 0; i < 10; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (i + 0x80) * 0x40, (void *)(0x06010000 + i * 0x80), 0x10);
        CpuFastSet(gUnknown_0200FC50 + (i + 0x90) * 0x40, (void *)(0x06010040 + i * 0x80), 0x10);
    }

    for (i = 0; i < 6; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (i + 0x8a) * 0x40, (void *)(0x06010500 + i * 0x80), 0x10);
        CpuFastSet(gUnknown_0200FC50 + (i + 0x9a) * 0x40, (void *)(0x06010540 + i * 0x80), 0x10);
    }

    for (i = 0; i < 4; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (i + 0xa0) * 0x40, (void *)(0x06010800 + i * 0x80), 0x10);
        CpuFastSet(gUnknown_0200FC50 + (i + 0xb0) * 0x40, (void *)(0x06010840 + i * 0x80), 0x10);
    }

    CpuFastSet(gUnknown_0200FC50 + 0x3b00, (void *)0x06010a00, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x3f00, (void *)0x06010a80, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x3900, (void *)0x06010b00, 0x10);
    CpuFastSet(gUnknown_0200FC50 + 0x3d00, (void *)0x06010b40, 0x10);
    CpuFastSet(gUnknown_0200FC50 + 0x3940, (void *)0x06010b80, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x3d40, (void *)0x06010c00, 0x20);
    CpuFastSet(gUnknown_0200FC50 + 0x3300, (void *)0x06010c80, 0x10);
    CpuFastSet(gUnknown_0200FC50 + 0x3700, (void *)0x06010cc0, 0x10);

    ApplyPaletteExt(gUnknown_0822AA80, 0x200, 0xc0);

    proc->unk58 = gUnknown_0202FDEC.unk0a;
    proc->unk5c = gUnknown_0202FDEC.unk0c;
    proc->unk4c = 0;
    proc->unk60 = 0x1e;
}
asm(".global sub_0807B360\n.thumb_set sub_0807B360, MatchSummaryPanel_LoadGraphics\n");
