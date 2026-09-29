#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800CFDC.
 * sub_0800CFDC @ 0x0800CFDC
 */

#include "map.h"
/* Bridge auto-tiling around map cell (x, y). Tiles 0x86/0x87 are bridge
 * pieces; MakeForestBlock2x2 and MakeForestBlock3x3 rewrite a 2x2 / 3x3 block whose
 * top-left corner they are given.
 *
 * Byte-matching notes (see docs/agbcc-codegen.md, MakeForestBlocks):
 * - Every cell read goes through TILE(), i.e. `rowOffset[y] + (x)` with the
 *   column parenthesised. `rowOffset[y] + x - 1` reassociates to
 *   `(row + x) - 1`; the ROM computes `(row - 1) + x`.
 * - `wroteLeft` must be set after the entry test: it is the ROM's `sl`.
 *   With it, the gcse PRE register for gMap's .rodata word loses the
 *   allocation contest, so every map read after the entry reloads gMap
 *   directly, as in the ROM.
 * - Case bodies are in the ROM's layout order, not numeric order.
 * - All exits share a `movs r0, #0` epilogue, so the function is `int`. */
#define BRIDGE(xx, yy) ((u16)(gMap->tile[gMap->rowOffset[(yy)] + (xx)] - 0x86) <= 1)
#define TILE(xx, yy) (gMap->tile[gMap->rowOffset[(yy)] + (xx)])

int MakeForestBlocks(int x, int y)
{
    int leftPair;
    int rightPair;
    int rightMask;
    int wroteLeft;

    if ((u16)(gMap->tile[gMap->rowOffset[y] + x] - 0x86) > 1)
        return 0;

    wroteLeft = 0;
    leftPair = x > 0 ? GetForestColumnMask(x - 1, y) : 0;
    rightPair = GetForestColumnMask(x, y);
    rightMask = x < gMap->width - 1 ? GetForestColumnMask(x + 1, y) : 0;
    leftPair &= rightPair;
    rightPair &= rightMask;

    /* Jump table at 0x0800D080. */
    switch (leftPair)
    {
    case 28:
    case 29:
    case 30:
    case 31:
        if (x > 0 && y > 0 &&
            BRIDGE(x - 1, y - 1) &&
            BRIDGE(x, y - 1) &&
            BRIDGE(x - 1, y))
        {
            MakeForestBlock2x2(x - 1, y - 1);
            wroteLeft = 1;
        }
        break;
    case 7:
    case 23:
        {
            int nextMask = GetForestBlockCorner(x, y + 1);
            if (nextMask == 1)
            {
                if (x < gMap->width - 2 && y < gMap->height - 2 &&
                    BRIDGE(x + 1, y) &&
                    BRIDGE(x + 2, y) &&
                    BRIDGE(x + 2, y + 1) &&
                    BRIDGE(x + 2, y + 2))
                {
                    MakeForestBlock3x3(x, y);
                    wroteLeft = 1;
                }
            }
            else if (nextMask == 2)
            {
                if (x > 0 && x < gMap->width - 1 &&
                    y < gMap->height - 2 &&
                    BRIDGE(x - 1, y) &&
                    BRIDGE(x + 1, y) &&
                    BRIDGE(x + 1, y + 1) &&
                    BRIDGE(x + 1, y + 2))
                {
                    MakeForestBlock3x3(x - 1, y);
                    wroteLeft = 1;
                }
            }
        }
        break;
    case 12:
    case 13:
        if (x > 0 && y > 0 &&
            BRIDGE(x - 1, y) &&
            BRIDGE(x, y) &&
            BRIDGE(x - 1, y - 1) &&
            BRIDGE(x, y - 1))
        {
            MakeForestBlock2x2(x - 1, y - 1);
            wroteLeft = 1;
        }
        break;
    case 6:
    case 22:
        if (x > 0 && y < gMap->height - 1 &&
            BRIDGE(x, y) &&
            BRIDGE(x - 1, y) &&
            BRIDGE(x - 1, y + 1) &&
            BRIDGE(x, y + 1))
        {
            MakeForestBlock2x2(x - 1, y);
            wroteLeft = 1;
        }
        break;
    }

    if (wroteLeft && leftPair == rightPair)
        return 0;

    /* Jump table at 0x0800D40C. */
    switch (rightPair)
    {
    case 28:
    case 29:
        if (x < gMap->width - 2 && y > 1 &&
            BRIDGE(x, y) && TILE(x + 1, y) == 0x65 &&
            TILE(x + 1, y - 1) == 0x45 &&
            TILE(x + 2, y) == 0x67 &&
            BRIDGE(x, y - 1) && BRIDGE(x, y - 2))
            MakeForestBlock3x3(x, y - 2);
        else if (x < gMap->width - 1 && y > 0 &&
                 BRIDGE(x, y) && BRIDGE(x + 1, y - 1) &&
                 BRIDGE(x, y - 1) && BRIDGE(x + 1, y))
            MakeForestBlock2x2(x, y - 1);
        break;

    case 15:
    case 31:
        if (x < gMap->width - 2 && y > 1 &&
            BRIDGE(x, y) && TILE(x + 2, y - 2) == 0x27 &&
            TILE(x + 2, y - 1) == 0x47 &&
            TILE(x + 1, y - 2) == 0x25 &&
            BRIDGE(x, y - 2) && BRIDGE(x, y - 1))
            MakeForestBlock3x3(x, y - 2);
        else if (x < gMap->width - 1 && y > 0 &&
                 BRIDGE(x, y) && BRIDGE(x, y - 1) &&
                 BRIDGE(x + 1, y - 1) && BRIDGE(x + 1, y))
            MakeForestBlock2x2(x, y - 1);
        break;

    case 30:
        if (x < gMap->width - 1 && y < gMap->height - 1 &&
            BRIDGE(x, y) && BRIDGE(x + 1, y) &&
            BRIDGE(x, y + 1) && BRIDGE(x + 1, y + 1))
            MakeForestBlock2x2(x, y);
        break;

    case 7:
    case 23:
        {
            int nextMask = GetForestBlockCorner(x, y + 1);
            if (nextMask == 1)
            {
                if (x < gMap->width - 2 && y < gMap->height - 2 &&
                    BRIDGE(x, y) && BRIDGE(x + 1, y) &&
                    BRIDGE(x + 2, y) && BRIDGE(x + 2, y + 1) &&
                    BRIDGE(x + 2, y + 2))
                    MakeForestBlock3x3(x, y);
            }
            else if (nextMask == 2 && x > 0 && x < gMap->width - 1 &&
                     y < gMap->height - 2 && BRIDGE(x, y) &&
                     BRIDGE(x - 1, y) && BRIDGE(x + 1, y) &&
                     BRIDGE(x + 1, y + 1) && BRIDGE(x + 1, y + 2))
                MakeForestBlock3x3(x - 1, y);
        }
        break;

    case 12:
    case 13:
        if (x < gMap->width - 1 && y > 0 &&
            BRIDGE(x, y) && BRIDGE(x + 1, y) &&
            BRIDGE(x + 1, y - 1) && BRIDGE(x, y - 1))
            MakeForestBlock2x2(x, y - 1);
        break;

    case 6:
    case 22:
        if (x < gMap->width - 1 && y < gMap->height - 1 &&
            BRIDGE(x, y) && BRIDGE(x + 1, y) &&
            BRIDGE(x + 1, y + 1) && BRIDGE(x, y + 1))
            MakeForestBlock2x2(x, y);
        break;
    }
    /* 0x0800D9D4: a complete 3x3 bridge neighbourhood centred on (x, y). */
    if (x > 0 && x < gMap->width - 1 && y > 0 && y < gMap->height - 1 &&
        BRIDGE(x, y) &&
        BRIDGE(x - 1, y - 1) && BRIDGE(x, y - 1) && BRIDGE(x + 1, y - 1) &&
        BRIDGE(x - 1, y) && BRIDGE(x + 1, y) &&
        BRIDGE(x - 1, y + 1) && BRIDGE(x, y + 1) && BRIDGE(x + 1, y + 1))
        MakeForestBlock3x3(x - 1, y - 1);

    /* 0x0800DAC6..DC32: north-facing corners. The y - 1 read is
     * unconditional in the ROM too. */
    if (!BRIDGE(x, y))
        return 0;
    switch (TILE(x, y - 1))
    {
    case 0x67:
        if (x > 0 && x < gMap->width - 1 && y > 1 &&
            (u16)(TILE(x - 1, y - 2) - 0x24) <= 1 &&
            BRIDGE(x + 1, y - 2) && BRIDGE(x + 1, y - 1) &&
            BRIDGE(x + 1, y) && BRIDGE(x - 1, y))
            MakeForestBlock3x3(x - 1, y - 2);
        break;
    case 0x64:
    case 0x65:
        if (x > 0 && x < gMap->width - 1 && y > 1 &&
            TILE(x + 1, y - 2) == 0x27 &&
            BRIDGE(x - 1, y - 2) && BRIDGE(x - 1, y - 1) &&
            BRIDGE(x - 1, y) && BRIDGE(x + 1, y))
            MakeForestBlock3x3(x - 1, y - 2);
        break;
    }

    /* 0x0800DC32..DE3A: the three east-of-north tile shapes. */
    switch (TILE(x + 1, y - 1))
    {
    case 0x67:
        if (x < gMap->width - 2 && y > 1 && BRIDGE(x, y) &&
            (u16)(TILE(x, y - 2) - 0x24) <= 1 &&
            BRIDGE(x + 2, y - 2) && BRIDGE(x + 2, y - 1) &&
            BRIDGE(x + 2, y) && BRIDGE(x + 1, y))
            MakeForestBlock3x3(x, y - 2);
        break;
    case 0x64:
    case 0x65:
        if (x < gMap->width - 2 && y > 1 && BRIDGE(x, y) &&
            TILE(x + 2, y - 2) == 0x27 && BRIDGE(x, y - 2) &&
            BRIDGE(x, y - 1) && BRIDGE(x + 1, y) && BRIDGE(x + 2, y))
            MakeForestBlock3x3(x, y - 2);
        break;
    case 0x24:
    case 0x25:
        if (x < gMap->width - 2 && y > 1 && BRIDGE(x, y) &&
            TILE(x + 2, y) == 0x67 && BRIDGE(x, y - 1) &&
            BRIDGE(x, y - 2) && BRIDGE(x + 1, y - 2) &&
            BRIDGE(x + 2, y - 2))
            MakeForestBlock3x3(x, y - 2);
        break;
    }

    /* 0x0800DE3A..DFB4: east patterns. */
    switch (TILE(x + 1, y))
    {
    case 0x24:
    case 0x25:
        if (x < gMap->width - 2 && y > 0 && y < gMap->height - 1 &&
            BRIDGE(x, y) && TILE(x + 2, y + 1) == 0x67 &&
            BRIDGE(x, y + 1) && BRIDGE(x, y - 1) &&
            BRIDGE(x + 1, y - 1) && BRIDGE(x + 2, y - 1))
            MakeForestBlock3x3(x, y - 1);
        break;
    case 0x64:
    case 0x65:
        if (x < gMap->width - 2 && y > 0 && y < gMap->height - 1 &&
            BRIDGE(x, y) && TILE(x + 2, y - 1) == 0x27 &&
            BRIDGE(x, y - 1) && BRIDGE(x, y + 1) &&
            BRIDGE(x + 1, y + 1) && BRIDGE(x + 2, y + 1))
            MakeForestBlock3x3(x, y - 1);
        break;
    }

    /* 0x0800DFB4..E1B6: east-of-south patterns. */
    switch (TILE(x + 1, y + 1))
    {
    case 0x27:
        if (x < gMap->width - 2 && y < gMap->height - 2 &&
            BRIDGE(x, y) && (u16)(TILE(x, y + 2) - 0x64) <= 1 &&
            BRIDGE(x + 1, y) && BRIDGE(x + 2, y) &&
            BRIDGE(x + 2, y + 1) && BRIDGE(x + 2, y + 2))
            MakeForestBlock3x3(x, y);
        break;
    case 0x24:
    case 0x25:
        if (x < gMap->width - 2 && y < gMap->height - 2 &&
            BRIDGE(x, y) && TILE(x + 2, y + 2) == 0x67 &&
            BRIDGE(x + 1, y) && BRIDGE(x + 2, y) &&
            BRIDGE(x, y + 1) && BRIDGE(x, y + 2))
            MakeForestBlock3x3(x, y);
        break;
    case 0x64:
    case 0x65:
        if (x < gMap->width - 2 && y < gMap->height - 2 &&
            BRIDGE(x, y) && TILE(x + 2, y) == 0x27 &&
            BRIDGE(x, y + 1) && BRIDGE(x, y + 2) &&
            BRIDGE(x + 1, y + 2) && BRIDGE(x + 2, y + 2))
            MakeForestBlock3x3(x, y);
        break;
    }

    /* 0x0800E1B6..E336: south-facing patterns, shifted left one cell. */
    switch (TILE(x, y + 1))
    {
    case 0x27:
        if (x > 0 && x < gMap->width - 1 && y < gMap->height - 2 &&
            BRIDGE(x, y) && (u16)(TILE(x - 1, y + 2) - 0x64) <= 1 &&
            BRIDGE(x - 1, y) && BRIDGE(x + 1, y) &&
            BRIDGE(x + 1, y + 1) && BRIDGE(x + 1, y + 2))
            MakeForestBlock3x3(x - 1, y);
        break;
    case 0x24:
    case 0x25:
        if (x > 0 && x < gMap->width - 1 && y < gMap->height - 2 &&
            BRIDGE(x, y) && TILE(x + 1, y + 2) == 0x67 &&
            BRIDGE(x - 1, y) && BRIDGE(x + 1, y) &&
            BRIDGE(x - 1, y + 1) && BRIDGE(x - 1, y + 2))
            MakeForestBlock3x3(x - 1, y);
        break;
    }

    /* 0x0800E336..E546: west-of-south mirrored corner patterns. */
    switch (TILE(x - 1, y + 1))
    {
    case 0x27:
        if (x > 1 && y < gMap->height - 2 && BRIDGE(x, y) &&
            (u16)(TILE(x - 2, y + 2) - 0x64) <= 1 &&
            BRIDGE(x - 2, y) && BRIDGE(x - 1, y) &&
            BRIDGE(x, y + 1) && BRIDGE(x, y + 2))
            MakeForestBlock3x3(x - 2, y);
        break;
    case 0x67:
        if (x > 1 && y < gMap->height - 2 && BRIDGE(x, y) &&
            (u16)(TILE(x - 2, y) - 0x24) <= 1 &&
            BRIDGE(x, y + 1) && BRIDGE(x, y + 2) &&
            BRIDGE(x - 1, y + 2) && BRIDGE(x - 2, y + 2))
            MakeForestBlock3x3(x - 2, y);
        break;
    case 0x24:
    case 0x25:
        if (x > 1 && y < gMap->height - 2 && BRIDGE(x, y) &&
            TILE(x, y + 2) == 0x67 &&
            BRIDGE(x - 2, y) && BRIDGE(x - 1, y) &&
            BRIDGE(x - 2, y + 1) && BRIDGE(x - 2, y + 2))
            MakeForestBlock3x3(x - 2, y);
        break;
    }

    /* 0x0800E546..E6B6: west-side corner and north/south bridge tiles. */
    switch (TILE(x - 1, y))
    {
    case 0x27:
        if (x > 1 && y > 0 && y < gMap->height - 1 &&
            BRIDGE(x, y) && (u16)(TILE(x - 2, y + 1) - 0x64) <= 1 &&
            BRIDGE(x - 2, y - 1) && BRIDGE(x - 1, y - 1) &&
            BRIDGE(x, y - 1) && BRIDGE(x, y + 1))
            MakeForestBlock3x3(x - 2, y - 1);
        break;
    case 0x67:
        if (x > 1 && y > 0 && y < gMap->height - 1 &&
            BRIDGE(x, y) && (u16)(TILE(x - 2, y - 1) - 0x24) <= 1 &&
            BRIDGE(x, y - 1) && BRIDGE(x, y + 1) &&
            BRIDGE(x - 1, y + 1) && BRIDGE(x - 2, y + 1))
            MakeForestBlock3x3(x - 2, y - 1);
        break;
    }

    /* 0x0800E6B6..E8BA: northwest corner and two-row bridge patterns. */
    switch (TILE(x - 1, y - 1))
    {
    case 0x27:
        if (x > 1 && y > 1 && BRIDGE(x, y) &&
            (u16)(TILE(x - 2, y) - 0x64) <= 1 &&
            BRIDGE(x - 2, y - 2) && BRIDGE(x - 1, y - 2) &&
            BRIDGE(x, y - 2) && BRIDGE(x, y - 1))
            MakeForestBlock3x3(x - 2, y - 2);
        break;
    case 0x67:
        if (x > 1 && y > 1 && BRIDGE(x, y) &&
            (u16)(TILE(x - 2, y - 2) - 0x24) <= 1 &&
            BRIDGE(x, y - 2) && BRIDGE(x, y - 1) &&
            BRIDGE(x - 1, y) && BRIDGE(x - 2, y))
            MakeForestBlock3x3(x - 2, y - 2);
        break;
    case 0x64:
    case 0x65:
        if (x > 1 && y > 1 && BRIDGE(x, y) &&
            TILE(x, y - 2) == 0x27 && BRIDGE(x - 2, y - 2) &&
            BRIDGE(x - 2, y - 1) && BRIDGE(x - 2, y) &&
            BRIDGE(x - 1, y))
            MakeForestBlock3x3(x - 2, y - 2);
        break;
    }
    return 0;
#undef TILE
#undef BRIDGE
}
asm(".global sub_0800CFDC\n.thumb_set sub_0800CFDC, MakeForestBlocks\n");
