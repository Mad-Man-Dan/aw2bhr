#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080085E0.
 * MakeTile @ 0x080085E0
 */

/* The tile-action dispatcher: read the map cell under the cursor, then run one
 * of nineteen handlers picked by the low five bits of gActiveMap->selectedTerrain,
 * leaving a result code in unk6a.
 *
 * gUnknown_0808D800 is NOT a global of the original source -- see the note on
 * it in unknown-globals.h. The source says `gActiveMap` and agbcc's
 * -fforce-addr parks the address in this unit's .rodata. The split can now
 * place that word directly, so the honest spelling below is byte-exact and
 * promotion must carry `rodata: ["0x0808D800"]`.
 *
 * The global is named again in every case arm rather than bound to a local:
 * the five reads before the switch share one deref because no call separates
 * them, but each arm re-derefs from scratch, which a local pointer could not
 * produce. x and y ARE locals -- they survive nineteen calls in r5/r6.
 *
 * Cases 9 and 18 are absent, not empty: their jump-table slots point at the
 * same epilogue the range check does.
 */

void MakeTile(void)
{
    int x, y;

    x = gActiveMap->cursorX;
    y = gActiveMap->cursorY;

    gActiveMap->cursorTerrain = gMap->tile[gMap->rowOffset[y] + x];
    gActiveMap->flags |= 0x1000;

    switch (gActiveMap->selectedTerrain & 0x1f) {
    case 7:
        MakeSeaSafest(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x8a;
        break;

    case 13:
        if (sub_0800B528(x, y) >= 0 && GetPropertyKindAt(x, y) != 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 1);
            MakeTile2(x, y, 1);
        }
        MakeShoal(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x4b;
        break;

    case 5:
        if (GetPropertyKindAt(x, y) != 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 1);
            MakeTile2(x, y, 1);
        }
        MakeRoad(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x89;
        break;

    case 2:
        if (GetPropertyKindAt(x, y) != 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 1);
            MakeTile2(x, y, 1);
        }
        if (MakeRiver(x, y) == 0)
            break;
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x2d;
        break;

    case 12:
        MakeBridge(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x89;
        break;

    case 3:
        if (GetPropertyKindAt(x, y) != 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 1);
            MakeTile2(x, y, 1);
        }
        sub_0800AF74(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x4b;
        break;

    case 19:
        if (MakeReefSafe(x, y) == 0)
            break;
        gActiveMap->soundId = 0x8a;
        break;

    case 4:
        MakeForest(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x4b;
        break;

    case 8:
        MakeProperty(x, y, gActiveMap->selectedTerrain);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x88;
        break;

    case 6:
    case 10:
    case 11:
    case 14:
    case 17:
        MakeProperty(x, y, gActiveMap->selectedTerrain);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x87;
        break;

    case 15:
        if (GetPropertyKindAt(x, y) != 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 1);
            MakeTile2(x, y, 1);
        }
        MakePipe(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x87;
        break;

    case 16:
        if (GetPropertyKindAt(x, y) != 0)
        {
            RemovePropertyAt(x, y);
            SetTerrainAt(x, y, 1);
            MakeTile2(x, y, 1);
        }
        MakeSeam(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x87;
        break;

    case 1:
        if (GetPropertyKindAt(x, y) != 0)
            RemovePropertyAt(x, y);
        SetTerrainAt(x, y, 1);
        MakeTile2(x, y, 1);
        sub_0800EC20(x, y);
        sub_0800BEE4(x, y);
        EnsureValidTile(x, y);
        gActiveMap->soundId = 0x4b;
        break;
    }
}

asm(".global sub_080085E0\n.thumb_set sub_080085E0, MakeTile\n");
