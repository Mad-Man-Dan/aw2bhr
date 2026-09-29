#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022428.
 * DrawUnitAt @ 0x08022428
 */

/* MATCHED (wave 52, W52-E), by decomp-permuter at iteration 553 from the 98.5%
 * hand draft. PROMOTION MUST CARRY THE POOL WORDS:
 *
 *     "rodata": ["0x0809099C", "0x080909A0"]
 *
 * then re-run tools/split_rodata.py and tools/gen_lds.py. gUnknown_0809099C
 * and gUnknown_080909A0 are agbcc's own -fforce-addr words for
 * gUnknown_08499590 and gUnits; the honest spelling names the
 * objects and the build places the two words (addends 0 and 4).
 *
 * THE WHOLE RESIDUAL WAS ONE REGISTER SWAP, and `(new_var = cx)` is what
 * closes it -- do not "clean it up". The 98.5% draft had
 *
 *     original            candidate
 *     mov r9, r0          mov sl, r0     <- cx
 *     mov sl, r0          mov r9, r0     <- cy
 *
 * plus the matching pair at the two uses. Writing the embedded assignment into
 * the first addend creates cx's reference at a point no statement boundary can
 * reach, which flips the two allocnos' priority order without moving either
 * definition. Measured alternatives, wave 52:
 *   - `gBG1TilemapBuffer + cy * 32 + cx` -- keeps the same registers and
 *     reorders the two uses instead. Worse.
 *   - swapping the `cx =` and `cy =` statements -- DOES flip r9/sl correctly,
 *     but then loads camY before camX. Trades 4 wrong bytes for 2.
 *   - declaration order of the two locals -- byte-identical either way. A
 *     local pseudo's allocno number comes from FIRST USE, not from
 *     expand_decl walking the declaration list.
 *
 * Two things were load-bearing before the permuter ever ran:
 *   - `cx`/`cy` hold the ALREADY-DOUBLED offsets and the address is
 *     `p + cx + cy * 32`. The exemplar's spelling (`cx = ... & 0xf` with
 *     `p + cx * 2 + cy * 64`, c_080223E0.c) sinks the doubling to the use as
 *     `lsls #2` / `lsls #7` and costs 4 bytes.
 *   - the guards must be ONE `||` chain with the drawing arm as the `else`;
 *     five separate `if (...) { blank; return; }` blocks lay the blank arm out
 *     after the drawing arm. */
void DrawUnitAt(u16 x, u16 y)
{
    struct Unit *e;
    u8 id;
    int cx;
    int cy;
    u16 v;
    u16 hp;
    unsigned char new_var;

    if (y >= gMap->height
        || (id = gMap
                     ->unitUnk[gMap->rowOffset[y] + x]) == 0
        || gMap
               ->unk234A[gMap->rowOffset[y] + x] == 0
        || !IsUnitIdVisibleToViewer(id)
        || (gUnits[id].flags & 4) != 0)
    {
        ClearUnitTileQuadAt(x, y);
    }
    else
    {
        if ((gUnits[id].flags & 1) != 0)
            v = 0x100;
        else
            v = id & 0xc0;

        cx = ((x - gMap->camX) & 0xf) * 2;
        cy = ((y - gMap->camY) & 0xf) * 2;

        e = &gUnits[id];

        if (e->hp != 0)
            hp = Div(e->hp - 1, 10) + 1;
        else
            hp = 0;

        WriteUnitTileQuad((gBG1TilemapBuffer + (new_var = cx)) + cy * 32,
                     gUnits[id].type,
                     v,
                     e->unk07 | e->unk08,
                     e->unk05_3,
                     hp,
                     (u8)(e->flags & 0x20),
                     GetPlayerSpecialAbilities((id >> 6) + 1) & 1);
    }
}

asm(".global sub_08022428\n.thumb_set sub_08022428, DrawUnitAt\n");
