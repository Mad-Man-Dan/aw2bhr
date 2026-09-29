#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D238.
 * sub_0803D238 @ 0x0803D238, sub_0803D2F8 @ 0x0803D2F8
 */

struct Rec
{
    /* 0x0000 */ u8 width;
    /* 0x0001 */ u8 height;
    /* 0x0002 */ u16 tile[(0x04B2 - 0x0002) / 2];
    /* 0x04B2 */ u8 name[0x11];
    /* 0x04C3 */ u8 unk4C3;
    /* 0x04C4 */ u8 unk4C4[5];
    /* 0x04C9 */ u8 unk4C9;
    /* 0x04CA */ u8 unk4CA;
    /* 0x04CB */ u8 cell[1];
};

/* The cell byte is read from the array at each use rather than bound to a
 * `u8` local, and that is what produces the ROM's `lsls #0x18` / `lsrs #0x1e`
 * pair. A `u8` local is PROMOTE_MODE'd to an SImode pseudo whose top bits
 * agbcc knows are zero, so `v >> 6` collapses to a single `lsr #6` and the
 * `!= 0` test compares the raw register. Reading through the record keeps the
 * value in QImode: the test needs `lsl #24` first, and the extract of bits
 * 6..7 then REUSES that shift (`lsr #30`). `& 0x3f` needs no extension in
 * either mode, which is why the low field stays a plain `and` in both.
 *
 * gUnknown_030033EC and gUnknown_03003F2C are saved and restored around the
 * call loop -- both spelled honestly. Their initial reads go through this
 * unit's own -fforce-addr `.rodata` words (the ROM words at 0x08091148 and
 * 0x0809114C, which hold &gUnknown_030033EC and &gUnknown_03003F2C -- checked
 * against baserom.gba, they are pool words and not globals), while the writes
 * get ordinary inline pool words. agbcc places both from the honest spelling;
 * nothing needs to name the 0x0809 addresses. The save locals are `int`, not
 * `u16`: the ROM stores them with `str` and reads them back with `ldrh`,
 * which is a word-wide local narrowed by the u16 store at the far end. */
void PlaceMapRecordUnits(u8 *a1)
{
    int save0;
    int save1;
    int x, y, k;

    save0 = gUnknown_030033EC;
    save1 = gUnknown_03003F2C;
    ClearAllUnits();
    k = 0;
    for (y = 0; y < gMap->height; y++) {
        for (x = 0; x < gMap->width; x++) {
            if (((struct Rec *)a1)->cell[k] != 0) {
                gUnknown_030033EC = (((struct Rec *)a1)->cell[k] >> 6) + 1;
                gUnknown_03003F2C = (((struct Rec *)a1)->cell[k] >> 6) << 6;
                CreateUnitAt(x, y, ((struct Rec *)a1)->cell[k] & 0x3f);
            }
            k++;
        }
    }
    gUnknown_030033EC = save0;
    gUnknown_03003F2C = save1;
}
asm(".global sub_0803D238\n.thumb_set sub_0803D238, PlaceMapRecordUnits\n");

/* The 5-byte copy loop REUSES `x` rather than taking a fresh `i`, and that is
 * a real byte difference, not a style choice: a fresh counter is a separate
 * pseudo and lands in r3 with the source base in r4, where the ROM has them
 * the other way round. Sharing the pseudo with the tile loop's column counter
 * puts it in r4 in both. Declaration order does not reach this -- moving `i`
 * to the end of the declaration list, into its own block, or binding the
 * source base to a named pointer were all measured and none moved it. */
void CopyMapRecordToGMap(int a1, u8 *a2)
{
    int x, y, k;

    CopyString((u8 *)a1, ((struct Rec *)a2)->name);
    gMap->unk4233 = ((struct Rec *)a2)->unk4C3;
    for (x = 0; x <= 4; x++)
        gUnknown_03003FF3[x] = ((struct Rec *)a2)->unk4C4[x];
    gMap->width = ((struct Rec *)a2)->width;
    gMap->height = ((struct Rec *)a2)->height;
    InitMapRowOffsets();
    k = 0;
    for (y = 0; y < gMap->height; y++) {
        for (x = 0; x < gMap->width; x++) {
            gMap
                ->tile[gMap->rowOffset[y] + x] =
                ((struct Rec *)a2)->tile[k];
            k++;
        }
    }
}
asm(".global sub_0803D2F8\n.thumb_set sub_0803D2F8, CopyMapRecordToGMap\n");
