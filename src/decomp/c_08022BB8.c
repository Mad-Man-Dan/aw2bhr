#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022BB8.
 * sub_08022BB8 @ 0x08022BB8
 */

/* Draws a small marker sprite near (x, y) for kinds 4, 5 and 6. The animation
 * is chosen by whether the position is past 0xCF horizontally and 0x8F / 0x7F
 * vertically; the offset table entry cycles with the game clock.
 *
 * Why the C looks odd: the on-screen tests read (s16)x / (s16)y into a fresh
 * temporary (one per test), while sx / sy hold the position shifted up by 16
 * bits for the later sprite call. The compiler then keeps the two forms in
 * separate registers, as the original does. */

void DrawMapCursorPointerSpriteUnused(s16 x, s16 y, s16 kind)
{
    struct UnkVec oam = { 0, 0 };
    u8 i;
    void *anim;
    u32 sx;
    u32 sy;
    int cx1;
    int cx4;
    int cx7;
    int cy2;
    int cy3;
    int cy5;
    int cy6;
    int cy8;
    int cy9;

    i = (u32)gGameClock % 0x21;

    if (i <= 4)
    {
        i = 0;
    }
    else if (i > 8)
    {
        if (i <= 0x1C)
            i = 4;
        else
            i = 2;
    }
    else
    {
        i = 2;
    }

    switch (kind)
    {
    case 4:
        cx1 = (s16)x;
        sx = (u32)(u16)x << 16;
        if (cx1 > 0xCF)
        {
            cy2 = (s16)y;
            sy = (u32)(u16)y << 16;
            if (cy2 > 0x8F)
            {
                i += 6;
                anim = gUnknown_08499BA0;
            }
            else
            {
                anim = gUnknown_08499BB4;
            }
        }
        else
        {
            cy3 = (s16)y;
            sy = (u32)(u16)y << 16;
            if (cy3 > 0x7F)
            {
                i += 0x12;
                anim = gUnknown_08499BC8;
            }
            else
            {
                i += 0xC;
                anim = gUnknown_08499B8C;
            }
        }
        ((struct OamData *)&oam)->tileNum = 0x365;
        ((struct OamData *)&oam)->paletteNum = 1;
        sub_0801C01C(((s32)sx >> 16) + gUnknown_080909B8[i],
                     ((s32)sy >> 16) + gUnknown_080909B8[i + 1], anim, oam, 1);
        break;
    case 5:
        cx4 = (s16)x;
        sx = (u32)(u16)x << 16;
        if (cx4 > 0xCF)
        {
            cy5 = (s16)y;
            sy = (u32)(u16)y << 16;
            if (cy5 > 0x8F)
            {
                i += 6;
                anim = gUnknown_08499BF0;
            }
            else
            {
                anim = gUnknown_08499C04;
            }
        }
        else
        {
            cy6 = (s16)y;
            sy = (u32)(u16)y << 16;
            if (cy6 > 0x7F)
            {
                i += 0x12;
                anim = gUnknown_08499C18;
            }
            else
            {
                i += 0xC;
                anim = gUnknown_08499BDC;
            }
        }
        ((struct OamData *)&oam)->tileNum = 0x365;
        ((struct OamData *)&oam)->paletteNum = 1;
        sub_0801C01C(((s32)sx >> 16) + gUnknown_080909B8[i],
                     ((s32)sy >> 16) + gUnknown_080909B8[i + 1], anim, oam, 1);
        break;
    case 6:
        cx7 = (s16)x;
        sx = (u32)(u16)x << 16;
        if (cx7 > 0xCF)
        {
            cy8 = (s16)y;
            sy = (u32)(u16)y << 16;
            if (cy8 > 0x8F)
            {
                i += 6;
                anim = gUnknown_08499C40;
            }
            else
            {
                anim = gUnknown_08499C54;
            }
        }
        else
        {
            cy9 = (s16)y;
            sy = (u32)(u16)y << 16;
            if (cy9 > 0x7F)
            {
                i += 0x12;
                anim = gUnknown_08499C68;
            }
            else
            {
                i += 0xC;
                anim = gUnknown_08499C2C;
            }
        }
        ((struct OamData *)&oam)->tileNum = 0x365;
        ((struct OamData *)&oam)->paletteNum = 1;
        sub_0801C01C(((s32)sx >> 16) + gUnknown_080909B8[i],
                     ((s32)sy >> 16) + gUnknown_080909B8[i + 1], anim, oam, 1);
        break;
    }
}
asm(".global sub_08022BB8\n.thumb_set sub_08022BB8, DrawMapCursorPointerSpriteUnused\n");
