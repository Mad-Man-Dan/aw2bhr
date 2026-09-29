#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077304.
 * sub_08077304 @ 0x08077304
 */

#include "proc.h"
#include "hardware.h"
/* MATCHED in wave 66 (W66-I). The two missing instructions diagnosed below
 * were the SAME mechanism at the two table-byte call sites: binding both
 * values to `register int arg asm("r2")` keeps each byte in r2 while agbcc
 * constructs argument 2, then emits the ROM's `adds r0, r2, #0` immediately
 * before the call. Fixing both also restored the ROM's r9/sl allocation.
 * Configured try_match verifies 796/796 bytes and equivalent relocations; the
 * promotion needs rodata 0x081CC5A0/5A4/5A8/5AC.
 *
 * HISTORICAL PARK (wave 54, W54-F) at 792/796 bytes -- FOUR bytes short, and the score
 * try_match prints (42.5%) is the positional artefact the brief warns about,
 * not a measure of how far this is. Everything in the function is derived and
 * byte-exact except ONE instruction and one register pair.
 *
 * REMAINING DIFF 1 -- the whole size difference, at +0x74, in the
 * `unk20 == NULL` arm:
 *
 *   ROM        adds r0, r2, #0 / adds r0, #0x3c / ldrb r2, [r0]
 *              ldr r4, =<forceaddr> / ldr r0, [r4] / ldr r1, [r0]
 *              lsls #0x1c / lsrs #0x1e / lsls #0xe / ldr r0, =0x060008C0
 *              adds r1, r1, r0 / adds r0, r2, #0 / movs r2, #1 / bl
 *   candidate  ... ldrb r0, [r0] ... ldr r1, [r4] / ldr r1, [r1] ...
 *              ldr r2, =0x060008C0 / adds r1, r1, r2 / movs r2, #1 / bl
 *
 * i.e. the ROM leaves the first argument in r2 and copies it into r0 after the
 * second argument is built, while this candidate coalesces it straight into r0
 * and the second argument uses r1 as its scratch. That is one `adds r0, r2, #0`
 * (2 bytes) plus the `.align` pad it costs the literal pool (2 more) -- the
 * -4 and the +0x74 first-difference are ONE fact, not two.
 *   Tried and made no difference: binding the byte to a fresh int local before
 *   the call (agbcc folds the local away and emits the identical stream).
 *
 * REMAINING DIFF 2 -- a high-register swap in the tail, size-neutral, about
 * eight bytes: the ROM puts the -fforce-addr word for gBG0TilemapBuffer in
 * `sb` and `&c` (the third CpuFastSet fill scalar) in `sl`; this candidate has
 * them the other way round, so every `mov rN, sb` reads `mov rN, sl`. Both
 * pseudos are created in the same preheader in the same order in both, so this
 * is allocno priority, not creation order.
 *
 * The permuter is NOT the tool here: the residual is a MISSING INSTRUCTION,
 * which docs/agbcc-codegen.md records as the one register-allocation shape it
 * cannot close. Fix diff 1 by hand first; diff 2 may well follow it, since
 * both point the same way (this candidate allocates one notch more tightly
 * than the ROM).
 *
 * EVERYTHING ELSE IS SETTLED AND SHOULD NOT BE RE-DERIVED:
 *  - The four pool words 0x081CC5A0/5A4/5A8/5AC are -fforce-addr words holding
 *    &gUnknown_0202FDFC, &gUnknown_03002B6C, &gBG0TilemapBuffer and
 *    &gUnknown_086145CE (read out of baserom.gba). Naming the objects directly
 *    reproduces every indirection level. A match needs
 *    "rodata": ["0x081CC5A0", "0x081CC5A4", "0x081CC5A8", "0x081CC5AC"].
 *  - gUnknown_085C77A0 is the 0x5c-stride record and struct Unk085C77A0
 *    already models unk18/unk3c[4]/unk40[4]; gUnknown_085C77DC is the SAME
 *    table 0x3c in (see the note added in include/unknown-globals.h).
 *  - `(s16)` on gUnknown_08615194[..].unk00 is what emits the `ldrsh`; the
 *    member stays u16 (wave 37: sign_extend of a mem:HI always folds to
 *    ldrsh, so the declared signedness is irrelevant).
 *  - unk10 and unk20 of struct Unk08615194 were carved out of filler here.
 *  - BOTH halfword store loops must bind the pointer global to a LOCAL first
 *    (W44-D). Written as `gBG0TilemapBuffer[0x29a + k]` the load is repeated
 *    inside the loop, strength reduction never fires and the loop stays
 *    ascending; bound to a local it collapses to the ROM's descending
 *    `strh / subs / subs / subs / cmp / bge`.
 *  - The two store loops and the unk40 scan need THREE DIFFERENT loop
 *    counters. Sharing one `i` gives it a live web spanning the calls, parks
 *    it in a callee-saved register and changes every register in both store
 *    loops (the W54-B fresh-name-per-site rule again).
 *  - The four CpuFastSet fill sources are plain `u32` scalars here, not the
 *    arrays W53-C describes: the value already lands in r4 and the address is
 *    materialised separately, which is the shape that chapter's array form
 *    exists to produce. Both a/b sites are byte-exact as written.
 */

struct Unk8077304Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ void *unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ void *unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 filler_40[0x04];
    /* 0x44 */ int unk44;
};

void WorldMapMissionInfo_Init(struct Unk8077304Proc * proc)
{
    int i;
    int j;
    int k;
    int m;
    register int arg asm("r2");
    u16 *p;
    u16 *q;
    u32 a;
    u32 b;
    u32 c;
    u32 d;

    gUnknown_0300064C = 0;
    SetVCountCompareLine(0xa0);
    SetVCountInterruptHandler((int)SplitScroll_BottomHandler);
    SetBgScrollShadow(0, 0, 0);
    SetBgScrollShadow(2, 0, 0);

    proc->unk34 = (s16)gUnknown_08615194[gUnknown_0202FDFC.unk0c].mapID;
    proc->unk2c = gUnknown_085C77A0[proc->unk34].unk18;
    proc->unk30 = gUnknown_08615194[gUnknown_0202FDFC.unk0c].unk10;

    if (gUnknown_08615194[gUnknown_0202FDFC.unk0c].coSelect == NULL)
    {
        arg = gUnknown_085C77A0[proc->unk34].unk3c[0];
        LoadCoFace(arg,
                     (void *)(0x060008C0 + (gUnknown_03002B6C.bits.chr_block << 14)),
                     1);
        proc->unk3c = 1;
    }
    else
    {
        Decompress(gUnknown_081D1F74,
                   (void *)(0x060008C0 + (gUnknown_03002B6C.bits.chr_block << 14)));
        ApplyPaletteExt(gUnknown_081D208C, 0x20, 0x20);
        proc->unk38 = gUnknown_08615194[gUnknown_0202FDFC.unk0c].coSelect;
        proc->unk3c = 0;
    }

    for (i = 0; i < proc->unk2c; i++)
    {
        if (gUnknown_085C77A0[proc->unk34].unk40[i] == 5)
        {
            arg = gUnknown_085C77A0[proc->unk34].unk3c[i];
            LoadCoMiniPortrait(arg,
                         (void *)0x06011300, 0x12);
        }
    }

    Decompress(gUnknown_081D35A8, (void *)0x06012000);
    ApplyPaletteExt(gUnknown_081D2264, 0x2a0, 0x20);
    Decompress(gUnknown_081D20EC,
               (void *)(0x06000520 + (gUnknown_03002B6C.bits.chr_block << 14)));

    a = 0;
    CpuFastSet(&a, gUnknown_08551A00, 0x01000070);
    b = 0;
    CpuFastSet(&b, gUnknown_08551A04, 0x01000070);
    c = 0;
    CpuFastSet(&c, gBG0TilemapBuffer, 0x01000200);
    d = 0;
    CpuFastSet(&d, gBG2TilemapBuffer, 0x01000200);

    if (gUnknown_0200C2D0[gUnknown_0202FDFC.unk0c].unk00[IsHardCampaignMode()].unk00_08 != 0)
    {
        j = 6 - GetRankFromScore(gUnknown_0200C2D0[gUnknown_0202FDFC.unk0c]
                                 .unk00[IsHardCampaignMode()].unk00_14);

        if (j != 0)
        {
            sub_0801F114();
            InitTilePool(4,
                         (void *)(0x06000000 + (gUnknown_03002B6C.bits.chr_block << 14)),
                         0x6a, 2);
            LoadTilePoolGraphic(gUnknown_086145CE[j]);

            p = gBG0TilemapBuffer;

            for (k = 0; k < 4; k++)
                p[0x29a + k] = 0x29 + k;

            PutTilePoolGraphicTilemap(gUnknown_086145CE[j], gBG0TilemapBuffer + 0x2bc);
        }
    }

    if (proc->unk3c != 0)
        PutTileBlock6x6(gUnknown_08551A00 + 1, 0x46, 1);
    else
        PutTileBlock6x4(gUnknown_08551A00 + 0x41, 0x46, 1);

    q = gBG0TilemapBuffer;

    for (m = 0; m < 4; m++)
        q[0x281 + m] = 0x2e + m;

    WorldMapMissionInfo_DrawPropertyCounts((struct Unk080772B8 *)(gBG0TilemapBuffer + 0x280));
    sub_080718F8(gUnknown_08551A04 + 0x20, gUnknown_081D2330, 0x360);
    sub_080718F8(gBG2TilemapBuffer + 0x280, gUnknown_081D249C, 0x360);

    proc->unk44 = 0;
}
asm(".global sub_08077304\n.thumb_set sub_08077304, WorldMapMissionInfo_Init\n");
