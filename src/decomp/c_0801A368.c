#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A368.
 * DrawWindowBackground @ 0x0801A368
 */

/* Draws a box `a4` rows tall into the tilemap buffer `a5` at column a1, row a2:
 * one top row, a4-2 middle rows alternating between two forms, one bottom row,
 * each 32 entries further on. If the buffer is one of the four known BG maps,
 * flag that BG for upload. */
void DrawWindowBackground(int a1, int a2, int a3, int a4, u16 *a5, int a6)
{
    u16 *dst;
    u16 i, alt;

    dst = a5 + a1 + a2 * 32;
    DrawWindowFrameTopRow(dst, a1, a3, a6);
    dst += 32;

    alt = 1;
    for (i = 0; i < a4 - 2; i++)
    {
        DrawWindowFrameMiddleRow(dst, a1, a3, alt, a6);
        dst += 32;
        alt = 1 - alt;
    }

    DrawWindowFrameBottomRow(dst, a1, a3, a6);

    if (a5 == gBG0TilemapBuffer)
        sub_08013AD4(0);

    if (a5 == gBG1TilemapBuffer)
        sub_08013AD4(1);

    if (a5 == gBG2TilemapBuffer)
        sub_08013AD4(2);

    if (a5 == gBG3TilemapBuffer)
        sub_08013AD4(3);

    SetMapLayerPrioritiesDefault();
}

asm(".global sub_0801A368\n.thumb_set sub_0801A368, DrawWindowBackground\n");
