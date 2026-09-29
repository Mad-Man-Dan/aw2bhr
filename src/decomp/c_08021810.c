#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08021810.
 * sub_08021810 @ 0x08021810, RecountArmyProperties @ 0x080219AC
 */

/* Reports two byte counts through out-pointers.  For the 0xB4..0xBF range of
 * gPlaySt.mapID the answers are canned in gUnknown_020280C0's record;
 * otherwise it loads the map, tallies the properties on it per owner into a
 * six-byte scratch, and reports (largest single army's tally, total).
 *
 * The guard is the same fold range test LoadMapIntoGMap carries, but on a u8
 * member: build_range_check keeps unsigned char as the working type, so the
 * subtraction truncates -- `adds #0x4c; lsls #0x18; lsrs #0x18; cmp #0xb; bhi`
 * is `unk02 >= 0xb4 && unk02 <= 0xbf` and nothing else.  (+0x4c is -0xb4 mod
 * 256; the ROM's own `subs #0xb4` in the index below is the same constant.)
 *
 * The two canned bytes are gUnknown_020280C0[i].filler_14[5] and [6], i.e.
 * +0x19 and +0x1a.  They are left inside filler_14 deliberately: the only other
 * reader/writer, src/decomp/c_0803D3F0.c, already writes exactly those two
 * indices from a save record, so naming them would strand that file for no
 * byte-level gain.
 *
 * The clear loop, the inner map loop and the max loop all run on ONE local `x`.
 * Declaring a separate `i` for the two short loops costs 4 bytes: x and the
 * `y * 2` temp swap r3/r4 and the pool address then needs an extra `mov r0, ip`
 * to load through.  RecountArmyProperties in this same block needed the identical fix;
 * see docs/agbcc-codegen.md.
 */

/* Rebuilds the two property lists from the map plane, in two passes over every
 * cell.  Pass one counts each army's properties into gPlayers[].unk11,
 * stamps the list index into the map's second 0x508-byte plane at +0x193A, and
 * fills gUnknown_084995A0[] with (terrain, x, y); pass two fills
 * gProperty[] with (raw cell, x, y).  Both lists are 0xFF-terminated,
 * and the second is capped at 0x5C entries -- the counter still advances past
 * the cap, so the terminator can land exactly at [0x5b].
 * The reset loop really is ascending: `subs r6,#1 / cmp r6,#0 / bge` at the
 * bottom is check_dbra_loop reversing an `x <= 4` counter whose only use is the
 * strength-reduced 0x3c byte stride.
 *
 * That reset loop runs on `x`, the SAME local as the inner map loop, and that
 * is worth 4 bytes: with a separate `i` declared, x lands in ip and
 * the map pointer in r6, which is the reverse of the ROM. Merging them adds
 * the reset loop's references to x's allocno and moves it ahead of the address
 * constant they were tying with. LoadMapAndCountProperties in this same block needed the
 * identical fix; see docs/agbcc-codegen.md.
 *
 * Case 16 in pass two falls THROUGH into the main group when
 * gPlaySt.gameMode == 5; the ROM's `bne` to the skip label plus a
 * fall-in to the shared block is exactly a case label with no break. */

void LoadMapAndCountProperties(u8 *a, u8 *b)
{
    u8 x;
    u8 y;

    if (gPlaySt.mapID >= 0xb4 && gPlaySt.mapID <= 0xbf)
    {
        *b = gUnknown_020280C0[gPlaySt.mapID - 0xb4].filler_14[5];
        *a = gUnknown_020280C0[gPlaySt.mapID - 0xb4].filler_14[6];
        return;
    }

    LoadMapIntoGMap(gPlaySt.mapID);

    for (x = 0; x <= 5; x++)
        gUnknown_030032D0[x] = 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            switch (gMap->terrain[gMap->rowOffset[y] + x] & 0x1f)
            {
            case TERRAIN_CITY:
            case TERRAIN_HQ:
            case TERRAIN_AIRPORT:
            case TERRAIN_PORT:
            case TERRAIN_BASE:
            case TERRAIN_LAB:
                gUnknown_030032D0[5]++;
                gUnknown_030032D0[
                    gMap->terrain[gMap->rowOffset[y] + x] >> 5]++;
                break;
            }
        }
    }

    gUnknown_030032D0[0] = 0;

    for (x = 0; x <= 4; x++)
    {
        if (gUnknown_030032D0[0] < gUnknown_030032D0[x])
            gUnknown_030032D0[0] = gUnknown_030032D0[x];
    }

    *b = gUnknown_030032D0[5];

    if (gUnknown_030032D0[0] < gUnknown_030032D0[5])
        gUnknown_030032D0[0]++;

    *a = gUnknown_030032D0[0];
}
asm(".global sub_08021810\n.thumb_set sub_08021810, LoadMapAndCountProperties\n");

void RecountArmyProperties(void)
{
    int n;
    int x;
    int y;

    n = 0;

    SetWorkingMapPlane(gMap->property);
    FillMovementMap(0xff);

    for (x = 0; x <= 4; x++)
        gPlayers[x].captures = 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            switch (gMap->terrain[gMap->rowOffset[y] + x] & 0x1f)
            {
            case TERRAIN_CITY:
            case TERRAIN_HQ:
            case TERRAIN_AIRPORT:
            case TERRAIN_PORT:
            case TERRAIN_BASE:
            case TERRAIN_SILO:
            case TERRAIN_LAB:
                gPlayers[
                    gMap->terrain[gMap->rowOffset[y] + x] >> 5].captures++;
                gMap->property[gMap->rowOffset[y] + x] = n;
                gUnknown_084995A0[n].unk00 =
                    gMap->terrain[gMap->rowOffset[y] + x] & 0x1f;
                gUnknown_084995A0[n].unk01 = x;
                gUnknown_084995A0[n].unk02 = y;
                gUnknown_084995A0[n].unk03[0] = 0;
                n++;
                break;
            }
        }
    }

    gUnknown_084995A0[n].unk00 = 0xff;
    gPlaySt.unk47 = 0;

    n = 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            switch (gMap->terrain[gMap->rowOffset[y] + x] & 0x1f)
            {
            case TERRAIN_PIPE_SEAM:
                if (gPlaySt.gameMode != 5)
                    break;
            case TERRAIN_CITY:
            case TERRAIN_HQ:
            case TERRAIN_AIRPORT:
            case TERRAIN_PORT:
            case TERRAIN_BASE:
            case TERRAIN_SILO:
            case TERRAIN_SILO_USED:
            case TERRAIN_LAB:
            case TERRAIN_MINICANNON_S:
            case TERRAIN_MINICANNON_N:
            case TERRAIN_MINICANNON_W:
            case TERRAIN_MINICANNON_E:
            case TERRAIN_LASER:
                if (n <= 0x5b)
                {
                    gProperty[n].flags =
                        gMap->terrain[gMap->rowOffset[y] + x];
                    gProperty[n].x = x;
                    gProperty[n].y = y;
                }

                n++;
                break;
            }
        }
    }

    if (n <= 0x5b)
        gProperty[n].flags = 0xff;
}

asm(".global sub_080219AC\n.thumb_set sub_080219AC, RecountArmyProperties\n");
