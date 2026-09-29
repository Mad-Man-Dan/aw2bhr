#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027FF4.
 * DayStartScreen_LoadGraphics @ 0x08027FF4
 */

/* The only thing this function's argument is used for is a `strh` at +0x20 --
 * a proc-like record whose extent is not visible from here.  Wave 56, W56-H. */
struct Unk08027FF4Proc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u16 unk20;
};

void DayStartScreen_LoadGraphics(struct Unk08027FF4Proc *proc)
{
    u16 n;
    u16 digit;
    int d;

    n = gUnknown_03004080;
    digit = 6;
    Decompress(gUnknown_08125A8C, gUnknown_0200FC50);
    Decompress(sub_0801F48C(), gUnknown_0200FC50 + 0x1400);
    do
    {
        CopyDayGlyphTiles(gUnknown_0200FC50, n % 10, digit);
        digit--;
        n /= 10;
    } while (n != 0);
    CopyDayGlyphTiles(gUnknown_0200FC50, 0xA, 0);
    CopyDayGlyphTiles(gUnknown_0200FC50, 0xB, 1);
    CopyDayGlyphTiles(gUnknown_0200FC50, 0xC, 2);
    CopyDayGlyphTiles(gUnknown_0200FC50, 0xD, 3);
    ApplyPaletteExt(gUnknown_081268B8, 0x260, 0x20);
    RegisterDataMove(gUnknown_080A1178, (void *)0x060045E0, 0xC0);
    ApplyPaletteExt(&gUnknown_080A1238[0][(gPlayers[gUnknown_030033EC].teamColor - 1) * 16],
                    0x120, 0x20);
    sub_08012B70(gBG0TilemapBuffer, gUnknown_080A12B8, 0, 7, 0x922F);
    BG_EnableSyncBG0();
    SetMapLayersBg0BlendFade();
    d = 1;
    if (gUnknown_03004080 > 9)
        d = 2;
    if (gUnknown_03004080 > 0x63)
        d = 3;
    proc->unk20 = (0x100 - ((int)GetDayWordGlyphCount() + d) * 24) >> 1;
}
asm(".global sub_08027FF4\n.thumb_set sub_08027FF4, DayStartScreen_LoadGraphics\n");
