#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086A58.
 * DrawMapList @ 0x08086A58
 *
 * Not a Xenesis-documented name. The old sub_08086A58 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

/* Draws `count` rows of the map list gUnknown_02027F74.unk04[], starting at
 * entry `first`, onto BG0 at tile rows top + 5, + 7, ... (two rows each). A map
 * id up to 0xB3 that is not unlocked yet is drawn as the gUnknown_084C3F50
 * placeholder label, anything else as its name. The last argument to
 * PutTextScriptImmediate is 1 when the map's bit in gUnknown_02028030.unk30 (tested by
 * HasMapBeenPlayed) is clear.
 *
 * In game mode 2 the rows start at top + 5 and DrawWarRoomRanks draws the rest of
 * the screen. Otherwise a header from gUnknown_08499CE4 goes on row 5, the rows
 * start at top + 9, and the count is clamped to the list's length
 * (unk37 + 1). DrawMapListFirstTwoRows is the same list drawn two rows at most.
 *
 * Measured spelling notes:
 * - The entries are read through the struct's own member array,
 *   gUnknown_02027F74.unk04[first + i]. That is what closed the function
 *   (97.6% -> match). A `u8 *` local set to (u8 *)&gUnknown_02027F74 and
 *   bumped by 4 lets strength reduction fold the base load into the entry
 *   pointer's giv. The ROM instead hoists the base as its own invariant, which
 *   puts its `ldr` ahead of the row counter's `movs #0` in the preheader. A
 *   local set to the member (`list = gUnknown_02027F74.unk04;`) is 4 bytes
 *   short.
 * - The row is `rowOffset = i * 2; top = ...; row = rowOffset + top;` in its
 *   own block in each arm (W85-D2). At function scope strength reduction
 *   turns the whole (s16) expression into one giv.
 * - The flag is set after the call from a block-local result:
 *   `unflagged = 1; if (bit != 0) unflagged = 0;`. A ?: or a preset before
 *   the call orders the `movs` differently (W85).
 */
void DrawMapList(int first, int count, int top)
{
    int i;
    int unflagged;
    int rowTop;
    int row;
    int unlocked;

    ClearBg0Tilemap();

    if (gPlaySt.gameMode == 2)
    {
        for (i = 0; i < count; i++)
        {
            {
                u8 bit = HasMapBeenPlayed(gUnknown_02027F74.unk04[first + i]);
                unflagged = 1;
                if (bit != 0)
                    unflagged = 0;
            }

            if (gUnknown_02027F74.unk04[first + i] <= 0xB3
                && (unlocked = (u8)IsCampaignMapUnlocked(gUnknown_02027F74.unk04[first + i])) == 0)
            {
                int rowOffset = i * 2;
                rowTop = top + 5;
                row = rowOffset + rowTop;
                PutTextScriptImmediate(1, (s16)row, gBG0TilemapBuffer,
                             gUnknown_084C3F50, unlocked, unflagged);
            }
            else
            {
                u8 *name = GetMapName(gUnknown_02027F74.unk04[first + i]);
                int rowOffset = i * 2;
                rowTop = top + 5;
                row = rowOffset + rowTop;
                PutTextScriptImmediate(1, (s16)row, gBG0TilemapBuffer, name, 0, unflagged);
            }
        }

        DrawWarRoomRanks(first, count, top);
    }
    else
    {
        PutTextTableEntryImmediate(1, 5, gBG0TilemapBuffer,
                     gUnknown_08499CE4[gUnknown_0300596C], 0, 0);

        if (gUnknown_02027F74.unk37 < count)
            count = gUnknown_02027F74.unk37 + 1;

        for (i = 0; i < count; i++)
        {
            {
                u8 bit = HasMapBeenPlayed(gUnknown_02027F74.unk04[first + i]);
                unflagged = 1;
                if (bit != 0)
                    unflagged = 0;
            }

            if (gUnknown_02027F74.unk04[first + i] <= 0xB3
                && (unlocked = (u8)IsCampaignMapUnlocked(gUnknown_02027F74.unk04[first + i])) == 0)
            {
                int rowOffset = i * 2;
                rowTop = top + 9;
                row = rowOffset + rowTop;
                PutTextScriptImmediate(1, (s16)row, gBG0TilemapBuffer,
                             gUnknown_084C3F50, unlocked, unflagged);
            }
            else
            {
                u8 *name = GetMapName(gUnknown_02027F74.unk04[first + i]);
                int rowOffset = i * 2;
                rowTop = top + 9;
                row = rowOffset + rowTop;
                PutTextScriptImmediate(1, (s16)row, gBG0TilemapBuffer, name, 0, unflagged);
            }
        }
    }

    BG_EnableSyncBG0();
}

asm(".global sub_08086A58\n.thumb_set sub_08086A58, DrawMapList\n");
