#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807AA84.
 * sub_0807AA84 @ 0x0807AA84
 */

#include "hardware.h"
#include "proc.h"
/* Wave 80 (W80-C): the one remaining ordering pair was a SOURCE CONSTRUCT.
 * In the first six-iteration copy loop the ROM loads the source base BEFORE
 * `i << 6`; in the second and third loops (and the else-arm loop) it shifts
 * first. With -fforce-addr an array's ADDR_EXPR is force_reg'd when the
 * operand is expanded, i.e. in source order, so the base load precedes the
 * shift UNLESS an `i * 0x40` pseudo already exists for CSE to reuse -- and
 * `dst = i * 0x40 + 0x06000800;` written first creates exactly that pseudo.
 * Loops 2/3 add a constant to the base (`+ 0x460`), which is a CONST that
 * emits nothing until the add, so there the shift is first either way.
 * The first loop therefore evaluates its source address before `dst`:
 * `src = gUnknown_0200FC50 + i * 0x40;` on the line before `dst`. W72-F's
 * "ordinary scoped pointer" measurement was against the older +4 residual
 * (the first-fill reload), not this one. */

/* WAVE 76: active high-water is now size-exact 1024/1024 at 98.9% (11 bytes
 * reported different). The only real instruction difference is one ordering
 * pair in the first six-iteration copy loop: ROM loads the source base before
 * i<<6, while agbcc schedules the independent shift first. Other displayed
 * differences are the expected 081D9304..081D9318 rodata symbol aliases. */

/* WAVE 72 (W72-F): configured baseline reverified at 1028/1024 (+4). The
 * advertised u16[3] aggregate, indexed and decayed-pointer call forms, nested
 * scalar initializer, comma/index assignment, and ordinary scoped pointer all
 * retain the extra first-fill reload or regress stack-slot placement. A
 * pointer/temporary chain can emit the ROM's address-zero-store sequence and
 * a fixed-register form reaches exact section size, but it globally rotates
 * scratch allocation and changes the first VRAM add to an OR, so it is weaker
 * than this readable baseline and was restored. Honest .rodata placement is
 * still correct; the open mechanism remains the first fill's one extra reload,
 * not a pool or transcription defect. */

/* WAVE 55 (W55-D) PARK at +4 bytes (1028 against 1024). W55-B's diagnosis is
 * CONFIRMED and sharpened below. The wave-55 orchestrator's handback diagnosis
 * -- "the draft emits its own .rodata where the ROM references named globals
 * gUnknown_081D9304/9308/930C/9318, fix the data spelling and it goes to 100%"
 * -- is REFUTED, and the refutation is the more useful result of the two.
 *
 * WHY THE DATA SPELLING IS ALREADY CORRECT
 * ----------------------------------------
 * The ROM's pool words are the splitter's invented names for agbcc's OWN
 * -fforce-addr address-constant blob at 0x081D9304..0x081D9318. This draft
 * names the real globals (gUnknown_0200FC50, +0x420, +0x460) and agbcc parks
 * their addresses in this unit's local .rodata, at the SAME relative offsets:
 *
 *     ROM  2bc: gUnknown_081D9310    candidate  2c0: .rodata +0xc
 *     ROM  2c0: gUnknown_081D9314    candidate  2c4: .rodata +0x10
 *     ROM  3e4: gUnknown_081D9318    candidate  3e8: .rodata +0x14
 *
 * 0x081D9304 + 0xc == 0x081D9310, and so on: the blob base lines up exactly.
 * That is the wave-18 honest spelling working as designed, and split_rodata.py
 * carves it at promote time -- it is NOT a defect and there is nothing to
 * respell. Naming gUnknown_081D9310 in the C would emit a word holding the
 * ADDRESS 0x081D9310 rather than the address constant stored AT it, which is a
 * different value and cannot match. The neighbouring promoted files cited as
 * worked examples (c_0807A268.c, c_0807AE94.c) do exactly what this draft
 * does: they name the real global and let the carve happen. This match will
 * need "rodata": ["0x081D9304" .. "0x081D9318"].
 *
 * THE ONE REAL DEFECT (wave 55): 2 BYTES AT +0x86, PLUS 2 OF POOL REALIGNMENT
 * -----------------------------------------------------------------
 * The first CpuSet's fill: ROM `mov r0,sp; movs r1,#0; mov r9,r1; strh` vs
 * candidate `movs r0,#0; mov sl,r0; mov r0,sp; mov r1,sl; strh` -- the
 * ORDER of two independent insns (address pseudo def vs const 0 def) inside
 * one block. Ruled out then: naming the 081D93xx symbols; binding the
 * address to a pointer local first (+8, live range across the call); local
 * declaration order; collapsing the proc pair. The register-asm block below
 * (wave 76) is what pinned it.
 *
 * TYPE NOTE, deliberately NOT acted on: gUnknown_0202FDEC.unk00[] and .unk04[]
 * are declared `u8` in include/unknown-globals.h and this function reads both
 * with `ldrsb`. The draft casts at the use rather than re-signing a shared
 * struct member. Byte-identical either way, so this function cannot settle it.
 */

struct Unk807AA84
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 filler_40[0x0c];
    /* 0x4c */ u16 unk4c;
};

void MatchSummary_Init(struct Unk807AA84 *proc)
{
    int i;
    int dst;
    u8 *src;
    u16 b;
    u16 c;

    SetupMenuScreenBgs(proc);
    BG_EnableSyncBG0();

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;
    gUnknown_03001FE8.bits.size = 1;
    gUnknown_030030B4.bits.size = 1;
    gUnknown_03002B34 = 0xF0;
    gUnknown_030030A0 = 0xFF10;
    gUnknown_0300251C.bits.tm_block = 0x1B;

    StartScrollingBackdrop(proc);

    {
        u16 a;
        register u16 *fill asm("r0") = &a;
        register u32 tmp asm("r1");
        register u32 tmp2 asm("r2");
        register u32 zero asm("r9");
        register u32 vram asm("r8");
        zero = tmp = 0;
        *fill = tmp;
        dst = gUnknown_03001FE8.bits.tm_block * 0x800;
        vram = 0x06000000;
        asm("" : "+r" (vram));
        dst += vram;
        asm("" : "+r" (dst));
    CpuSet(fill, (void *)dst, 0x01000400);

    gUnknown_03001FE8.bits.chr_block = 1;
    gUnknown_030030B4.bits.chr_block = 1;
    gUnknown_03001FE8.bits.tm_block = 0x1E;
    gUnknown_030030B4.bits.tm_block = 0xC;

    fill = &b;
    asm("" : "+r" (fill));
    tmp = zero;
    *fill = tmp;
    dst = gUnknown_03001FE8.bits.tm_block * 0x800;
    dst += vram;
    asm("" : "+r" (dst));
    CpuSet(fill, (void *)dst, 0x01000800);
    fill = &c;
    asm("" : "+r" (fill));
    tmp2 = zero;
    *fill = tmp2;
    CpuSet(fill, (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + vram), 0x01000800);

    if (gUnknown_0202FDEC.unk09 == 0)
        proc->unk3c = 1;
    else
        proc->unk3c = 0;

    if (proc->unk3c == 0)
    {
        Decompress(gUnknown_0822AC80,
                   (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06000000));
        Decompress(gUnknown_0822B944, gUnknown_0200FC50);
        CpuFastSet(gUnknown_0200FC50,
                   (void *)(gUnknown_03001FE8.bits.tm_block * 0x800 + 0x06000000), 0xC0);

        for (i = 0; i < 6; i++)
        {
            src = gUnknown_0200FC50 + i * 0x40;
            dst = i * 0x40 + 0x06000800;
            CpuFastSet(src,
                       (void *)(gUnknown_03001FE8.bits.tm_block * 0x800 + dst), 8);
            CpuFastSet(src,
                       (void *)(gUnknown_03001FE8.bits.tm_block * 0x800 + dst + 0x20), 8);
        }

        CpuFastSet(gUnknown_0200FC50 + 0x440,
                   (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + 0x06000440), 0x30);

        for (i = 0; i < 3; i++)
        {
            dst = i * 0x40 + 0x06000C40;
            CpuFastSet(gUnknown_0200FC50 + 0x460 + i * 0x40,
                       (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + dst), 8);
            CpuFastSet(gUnknown_0200FC50 + 0x460 + i * 0x40,
                       (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + dst + 0x20), 8);
        }

        ApplyPaletteExt(gUnknown_0822BB60, 0, 0x20);

        for (i = 0; i < gUnknown_0202FDEC.unk08; i++)
            LoadCoFace(gPlayers[(s8)gUnknown_0202FDEC.unk00[i]].co + 0x18,
                         (void *)(((i * 0x24 + 0x100) & 0x3FF) * 0x20 + 0x06010000),
                         i + 0x19);

        for (i = 0; i < gUnknown_0202FDEC.unk09; i++)
            LoadCoFace(gPlayers[(s8)gUnknown_0202FDEC.unk04[i]].co + 0x30,
                         (void *)(((i * 0x24 + 0x16C) & 0x3FF) * 0x20 + 0x06010000),
                         i + 0x1C);
    }
    else
    {
        Decompress(gUnknown_0822BB80,
                   (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06000000));
        Decompress(gUnknown_0822BCF0, gUnknown_0200FC50);
        CpuFastSet(gUnknown_0200FC50 + 0x400,
                   (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + 0x06000400), 0x40);

        for (i = 0; i < 4; i++)
        {
            dst = i * 0x40 + 0x06000C00;
            CpuFastSet(gUnknown_0200FC50 + 0x420 + i * 0x40,
                       (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + dst), 8);
            CpuFastSet(gUnknown_0200FC50 + 0x420 + i * 0x40,
                       (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + dst + 0x20), 8);
        }

        ApplyPaletteExt(gUnknown_0822BDFC, 0, 0x20);

        for (i = 0; i < gUnknown_0202FDEC.unk08; i++)
            LoadCoFace(gPlayers[(s8)gUnknown_0202FDEC.unk00[i]].co + 0x30,
                         (void *)(((i * 0x24 + 0x100) & 0x3FF) * 0x20 + 0x06010000),
                         i + 0x19);
    }

    InitTextTileCache(0);
    proc->unk4c = 0;
    }
}
asm(".global sub_0807AA84\n.thumb_set sub_0807AA84, MatchSummary_Init\n");
