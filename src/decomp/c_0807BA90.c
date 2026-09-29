#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BA90.
 * sub_0807BA90 @ 0x0807BA90
 */

#include "hardware.h"
/* VERIFIED MATCH (wave 54, W54-A) -- try_match reports matched, 432/432 bytes.
 *
 * PROMOTION NOTE: needs two -fforce-addr pool words placed.
 * data/promoted.json entry must carry:  "rodata": ["0x081D932C", "0x081D9330"]
 * 0x081D932C holds 0x030030B4 (&gUnknown_030030B4) and 0x081D9330 holds
 * 0x0300592C (&gUnknown_0300592C). Neither is a global -- see the 081D93xx
 * block in include/unknown-globals.h.
 *
 * LOAD-BEARING SPELLING, and it was the whole diff (36.6% before, matched
 * after): the running-sum loop must walk an explicit `u8 *`, NOT a subscript.
 *
 *     u8 *p = gUnknown_0202FF78;
 *     for (i = 0; i < 30; i++) { p[1] = p[1] + p[0] + 1; p++; }
 *
 * Spelled with subscripts -- `gUnknown_0202FF78[i + 1] = gUnknown_0202FF78[i]
 * + gUnknown_0202FF78[i + 1] + 1` -- agbcc emits an ASCENDING counter
 * (`add r2,#1; cmp r2,#0x1d; ble`) where the ROM has the reversed
 * `mov r2,#0x1d; sub r2,#1; cmp r2,#0; bge`. With the explicit pointer,
 * check_dbra_loop fires and the counter reverses. The reason is that a
 * subscript makes `i` the base of the address giv, so the biv cannot be
 * reversed; an explicit pointer makes `p` its own biv and leaves `i` a pure
 * trip counter, which is reversible. The source direction is irrelevant --
 * ascending and descending pointer versions compile to the same reversed
 * counter. See the chapter added to docs/agbcc-codegen.md.
 *
 * Also note `p[1] + p[0]`, not `p[0] + p[1]`: the SECOND operand is loaded
 * first, so the ROM's `ldrb [r4]` before `ldrb [r4,#1]` needs p[1] written on
 * the left.
 *
 * The four stack-address spills in the ROM's preheader (sp+0xe/0x10/0x12/0x14
 * saved into sp+0x18..0x24) are register-allocator output for the
 * BgAffineSrcData fields, not source; they fell into place with the loop fix
 * and needed no authoring.
 */

struct Unk0807BA90Proc
{
    /* 0x00 */ u8 unk00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 unk30[4];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 unk38[0x14];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u8 unk4e[0xa];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 unk60[4];
    /* 0x64 */ u16 unk64;
};

void MissionTitleName_Init(struct Unk0807BA90Proc * proc)
{
    int i;
    u8 * p;
    struct BgAffineSrcData src;

    Decompress(gUnknown_0822BE1C, (void *)((gUnknown_030030B4.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_0822D888, gBG2TilemapBuffer);

    BG_EnableSyncBG2();

    gUnknown_0300592C = sub_0807B7BC(GetMapName(gPlaySt.mapID), 0, gUnknown_0202FF78, 0x80, proc);
    gUnknown_0202FF78[0] = 0;

    p = gUnknown_0202FF78;

    for (i = 0; i < 30; i++)
    {
        p[1] = p[1] + p[0] + 1;
        p++;
    }

    proc->unk4c = gUnknown_0300592C;

    ApplyPaletteExt(gUnknown_0822DE80, 0x200, 0x20);

    gDispIo.disp_ct.mode = 1;
    gDispIo.disp_ct.bg3_enable = 0;

    gUnknown_030030B4.bits.color_depth = 1;
    gUnknown_030030B4.bits.size = 1;

    SetObjAffine(1,
                 Div(gSinLut[0x40] << 4, 0x200),
                 Div(-gSinLut[0] << 4, 0x200),
                 Div(gSinLut[0] << 4, 0x200),
                 Div(gSinLut[0x40] << 4, 0x200));

    proc->unk2c = 0x1e0;
    proc->unk5c = 0x6a;
    proc->unk58 = 0;
    proc->unk64 = 0xf0;
    proc->unk34 = 0;

    src.texX = 0x5000;
    src.texY = 0x5000;
    src.scrX = proc->unk2c;
    src.scrY = 0x50;
    src.sx = proc->unk5c;
    src.sy = proc->unk5c;
    src.alpha = proc->unk58;

    BgAffineSet(&src, (struct BgAffineDstData *)gUnknown_030024D0, 1);
}
asm(".global sub_0807BA90\n.thumb_set sub_0807BA90, MissionTitleName_Init\n");
