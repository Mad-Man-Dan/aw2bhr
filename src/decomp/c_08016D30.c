#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016D30.
 * sub_08016D30 @ 0x08016D30, sub_08016DB8 @ 0x08016DB8
 */

/* The save half of the pair LoadSuspendSave loads back: flush slot `a`'s block
 * into gUnknown_02000000. The same `(u8)(unk02 + 0x4c) <= 0xb` wrapping range
 * test on the mode byte guards the SaveDesignRoomSlot call there too.
 *
 * `a` is a u16 the body reads BOTH ways: `(s8)a` for the GetSuspendFlag /
 * SetSuspendFlag pair (both take s8) and `(u8)a` for sub_0801A7D8, and agbcc
 * shares the single `lsls #0x18` between them -- the ROM's `lsls r5,r4,#0x18;
 * asrs r4,r5,#0x18` ... `lsrs r0,r5,#0x18` is one shifted value with two
 * extensions, not two casts. The bare `lsls #0x18; cmp #0` on GetSuspendFlag's
 * result is the truth test of its s8 return. */
void WriteSuspendSave(u16 a, u8 b)
{
    if (a != 0 && gPlaySt.savingEnabled == 0) {
        MarkProfileSaved();
        if (GetSuspendFlag(a) == 0)
            SetSuspendFlag(a, 1);
        if ((u8)(gPlaySt.mapID + 0x4c) <= 0xb)
            SaveDesignRoomSlot(3, gMap->unk421a, 1);
        sub_08016F38(b);
        sub_0801A7D8(a, gUnknown_02000000, 0xE28);
        if (gPlaySt.gameMode == 1)
            BackupBattleMapPoints();
    }
}
asm(".global sub_08016D30\n.thumb_set sub_08016D30, WriteSuspendSave\n");

/* The load half of the save/load pair at WriteSuspendSave: restore slot `a`'s
 * block out of gUnknown_02000000 and rebuild from it. The `(u8)a` narrowing at
 * the ReadSaveSlot call is the callee's declared u8 parameter, not a cast --
 * the surviving `lsls #0x10; lsrs #0x10` at entry is the u16 one.
 *
 * `(u8)(gPlaySt.mapID + 0x4c) <= 0xb` is a wrapping RANGE TEST on the
 * mode byte, and the u8 cast is what makes it `lsls #0x18; lsrs #0x18; cmp
 * #0xb; bhi` rather than a pair of signed compares. Same guard as WriteSuspendSave's
 * SaveDesignRoomSlot call, one address block up. */
void LoadSuspendSave(u16 a)
{
    if (a != 0) {
        ReadSaveSlot(a, gUnknown_02000000);
        RestoreBattleSaveState();
        if ((u8)(gPlaySt.mapID + 0x4c) <= 0xb)
            LoadSavedMapRecordIntoGMap(3, (int)gMap->unk421a);
        RebuildTerrainPlaneFromTiles();
    }
}
asm(".global sub_08016DB8\n.thumb_set sub_08016DB8, LoadSuspendSave\n");
