#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BCF0.
 * sub_0802BCF0 @ 0x0802BCF0, sub_0802BD54 @ 0x0802BD54, sub_0802BDBC @ 0x0802BDBC, sub_0802BE28 @ 0x0802BE28, sub_0802BE80 @ 0x0802BE80, sub_0802BEC4 @ 0x0802BEC4, sub_0802BF20 @ 0x0802BF20
 */

#include "hardware.h"
/* Wipe-transition setup, "closing" direction: clears the two scroll globals,
 * loads the tile source into the char base block the BGCNT shadow currently
 * selects, paints all fifteen columns at phase 0, and resets the proc counter.
 *
 * `lsls #0x1c; lsrs #0x1e` on gUnknown_03002B6C is bits 3:2, i.e.
 * `bits.chr_block`, and `<< 14` turns the block index into its VRAM offset --
 * the same idiom c_0800572C.c and c_08005874.c already use on this shadow. It
 * is a genuine bitfield read, not a hand-written shift pair, and hardware.h has
 * to be included for it: `global.h` does not pull that header in, and declaring
 * the symbol as a scalar in unknown-globals.h instead conflicts with the union
 * and breaks every compile in the repo. */

struct Unk2BE28
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};
/* The opening twin of ScreenCoverWipe_Init: same Decompress into the char base block
 * the BGCNT shadow selects (`gUnknown_03002B6C.bits.chr_block << 14`, bits 3:2)
 * and the same fifteen-column paint, but it does not clear the two scroll
 * globals and it paints at the far end of the phase range (6) with the flip
 * flag off. */

struct Unk2BE80
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};
/* Per-frame step of the wipe: repaints columns 0 .. min(unk1e, 14) with a phase
 * that falls by one per column, then advances unk1e until it reaches 0x15, at
 * which point the proc's unk08 word is cleared instead.
 *
 * `phase` MUST be `s16`, not `u16`. agbcc's PROMOTE_MODE keeps a short local
 * ZERO-extended -- that is the `lsrs r6, r0, #0x10` defining it -- and
 * sign-extends it again at every use, which is the `lsls #0x10; asrs #0x10`
 * feeding the call. Keeping it in that shifted representation is also what
 * turns `phase--` into `(phase << 16) + 0xFFFF0000 >> 16`; a `u16` phase
 * decrements as a plain `subs #1` before the truncation instead, which is one
 * instruction and one pool word shorter.
 *
 * unk1e is re-read from memory at both loop tests (`ldrsh` every pass) even
 * though nothing in the loop writes it -- that is the field access itself, not
 * a cached local, and caching it in a local loses the reload. */

struct Unk2BEC4
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u8 filler_0c[0x1e - 0x0c];
    /* 0x1e */ s16 unk1e;
};
/* The opening counterpart of ScreenCoverWipe_Loop: same per-frame column repaint and
 * same unk1e advance, but the phase runs the other way -- it starts at
 * `6 - unk1e` and rises -- and the flip flag is 0. The two loop tests are `<=`
 * here where ScreenCoverWipe_Loop has `<`, which is the `bgt` / `bge` difference.
 *
 * `phase` is `s16` for the same reason as in ScreenCoverWipe_Loop: PROMOTE_MODE stores
 * it zero-extended (`lsrs r6, r0, #0x10`) and sign-extends it at the call
 * (`lsls #0x10; asrs #0x10`), and keeping it in that shifted representation is
 * what turns `phase++` into `(phase << 16) + 0x10000 >> 16` rather than a plain
 * `adds #1` before the truncation. */

struct Unk2BF20
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u8 filler_0c[0x1e - 0x0c];
    /* 0x1e */ s16 unk1e;
};

/* Renders `value` as decimal, right to left, one 8x8 glyph per digit stepping 7
 * pixels left each time. `value` is UNSIGNED -- the ROM's divide and remainder
 * are __udivsi3 / __umodsi3, and an `int` gives __divsi3 / __modsi3.
 *
 * The exit MUST be spelled `return`, not `break`. With `break` agbcc rotates the
 * loop: the `x -= 7` moves to the top behind an entry `b`, x is forced into a
 * high register and y follows it, and the function grows a third callee-saved
 * high register. With `return` the body stays at the top and the exit falls
 * straight into the epilogue, which is the ROM. (Same rule as FindMapIdByMapData in
 * this wave, now confirmed on a `for(;;)` rather than a `while`.)
 *
 * The `x -= 7` sits AFTER the test, so the last digit does not step: that is why
 * the loop bottom is `cmp; beq; ...; b` and not a single `bne`.
 *
 * gUnknown_0849A3B8 is named honestly and the two-level chain reproduces on its
 * own -- agbcc parks its address in this unit's .rodata, which is the ROM word
 * at 0x08090BC8 that gen_lds.py calls gUnknown_08090BC8. That word and
 * gUnknown_08090BCC (DrawSpriteNumberFont2's) BOTH hold 0x0849A3B8: they are two
 * -fforce-addr words for one symbol, not two different tables. */
void DrawSpriteNumberFont1(u16 x, u16 y, u32 value)
{
    for (;;)
    {
        PutOamHi(x & 0x1FF, y & 0xFF, gUnknown_0849A3B8, value % 10);
        value /= 10;
        if (value == 0)
            return;
        x -= 7;
    }
}
asm(".global sub_0802BCF0\n.thumb_set sub_0802BCF0, DrawSpriteNumberFont1\n");

/* DrawSpriteNumberFont1's twin: the same right-to-left decimal renderer, differing only
 * in that the glyph id is offset by 0x10 (a second digit font in the same
 * gUnknown_0849A3B8 table). See c_0802BCF0.c for the `return`-not-`break` rule
 * and for the two -fforce-addr words that both hold 0x0849A3B8. */
void DrawSpriteNumberFont2(u16 x, u16 y, u32 value)
{
    for (;;)
    {
        PutOamHi(x & 0x1FF, y & 0xFF, gUnknown_0849A3B8, value % 10 + 0x10);
        value /= 10;
        if (value == 0)
            return;
        x -= 7;
    }
}
asm(".global sub_0802BD54\n.thumb_set sub_0802BD54, DrawSpriteNumberFont2\n");

/* Paints one column of the wipe transition: a 2x2 tile block per row over ten
 * rows of the gBG0TilemapBuffer tilemap, `col * 4` bytes in and 0x80 bytes down
 * per iteration. `w` is the H-FLIP flag -- it is both the `<< 10` bit of the map
 * entry and the +0/+1 that swaps the left and right tiles of each pair, which is
 * why it appears twice in every value.
 *
 * `p[1]` MUST be spelled `base - (u16)(w - 1)`. Without the cast, fold's
 * split_tree pulls the constant out of `w - 1` and re-associates the whole thing
 * into `(base + 1) - w`, which is exactly p[0x21]'s tree, so CSE collapses the
 * two into one value: 4 bytes short, and r7 never gets pushed. The cast is a
 * fold barrier and costs nothing at run time -- the truncation is dead because
 * the value is only ever stored with `strh`, so combine drops the shift pair.
 *
 * The four values are written INSIDE the loop and hoisted by LICM, not written
 * above it: the ROM's `movs r4, #0` (the `for` init) comes BEFORE all four,
 * which is the preheader boundary.
 *
 * The pool word is 0xFFFF82B0, i.e. the constant is NEGATIVE as a 32-bit value;
 * `+ 0x82B0` emits a positive word and does not match. Whether the original
 * wrote `- 0x7D50` or `(s16)0x82B0` is not recoverable. */
void SetScreenWipeColumn(u8 col, s16 phase, u16 w)
{
    u16 *p;
    u16 base;
    u8 i;

    if (phase > 5)
        phase = 6;
    if (phase < 0)
        phase = 0;
    p = gBG0TilemapBuffer + col * 2;
    base = phase * 4 - 0x7D50 + (w << 10);
    for (i = 0; i <= 9; i++)
    {
        p[0] = base + w;
        p[1] = base - (u16)(w - 1);
        p[0x20] = base + w + 2;
        p[0x21] = base + 1 - w + 2;
        p += 0x40;
    }
}
asm(".global sub_0802BDBC\n.thumb_set sub_0802BDBC, SetScreenWipeColumn\n");

void ScreenCoverWipe_Init(struct Unk2BE28 *p)
{
    u8 col;

    gUnknown_03001418 = 0;
    gUnknown_03001FF8 = 0;
    Decompress(gUnknown_0810BDC0, (void *)(0x06005600 + (gUnknown_03002B6C.bits.chr_block << 14)));
    for (col = 0; col <= 0xE; col++)
        SetScreenWipeColumn(col, 0, 1);
    p->unk1e = 0;
}
asm(".global sub_0802BE28\n.thumb_set sub_0802BE28, ScreenCoverWipe_Init\n");

void ScreenRevealWipe_Init(struct Unk2BE80 *p)
{
    u8 col;

    Decompress(gUnknown_0810BDC0, (void *)(0x06005600 + (gUnknown_03002B6C.bits.chr_block << 14)));
    for (col = 0; col <= 0xE; col++)
        SetScreenWipeColumn(col, 6, 0);
    p->unk1e = 0;
}
asm(".global sub_0802BE80\n.thumb_set sub_0802BE80, ScreenRevealWipe_Init\n");

void ScreenCoverWipe_Loop(struct Unk2BEC4 *p)
{
    u8 col;
    s16 phase;

    phase = p->unk1e;
    for (col = 0; col < p->unk1e && col <= 0xE; col++)
        SetScreenWipeColumn(col, phase--, 1);
    BG_EnableSyncBG0();
    if (p->unk1e == 0x15)
        p->unk08 = 0;
    else
        p->unk1e++;
}
asm(".global sub_0802BEC4\n.thumb_set sub_0802BEC4, ScreenCoverWipe_Loop\n");

void ScreenRevealWipe_Loop(struct Unk2BF20 *p)
{
    u8 col;
    s16 phase;

    phase = 6 - p->unk1e;
    for (col = 0; col <= p->unk1e && col <= 0xE; col++)
        SetScreenWipeColumn(col, phase++, 0);
    BG_EnableSyncBG0();
    if (p->unk1e == 0x15)
        p->unk08 = 0;
    else
        p->unk1e++;
}
asm(".global sub_0802BF20\n.thumb_set sub_0802BF20, ScreenRevealWipe_Loop\n");
