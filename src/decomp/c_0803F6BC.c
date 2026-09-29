#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F6BC.
 * sub_0803F6BC @ 0x0803F6BC
 */

/* The per-terrain sprite loader behind src/decomp/c_0803F5E4.c: it opens with
 * the SAME `src` / `table` pointer pair that matched LoadMapObjectGraphics uses, which
 * is what puts both tables in callee-saved registers before the a4 guard and
 * turns every arm's offset into a run-time `movs; lsls; adds`. Naming the two
 * globals inline instead folds each offset into its own relocated pool word and
 * loses r6/r8 entirely.
 *
 * THE ARM ORDER IS THE SOURCE ORDER and it is not ascending: agbcc emits case
 * bodies exactly where they appear, so the ROM's block layout
 * (default, 8, 11, 10, 6, 14, 17) reads the original listing straight off. */
void LoadTerrainObjTiles(int a1, int a2, void *a3, int a4)
{
    u8 *src = gUnknown_080CFFC4;
    u8 *table = gUnknown_080D16C4;

    if (a4 == 0)
        return;

    switch (a1)
    {
    default:
        RegisterDataMove((void *)(GetTerrainPictureGraphic(a1, 0) + 0x40), a3, 0xc0);
        RegisterDataMove(gUnknown_08485A2C, (u8 *)a3 + 0xc0, 0x40);
        break;
    case 8:
        RegisterDataMove(table + ((((GetPlayerCoCountry(a2) - 1) << 3) & 0x3ff) << 5),
                     a3, 0x100);
        break;
    case 11:
        RegisterDataMove(src + 0x240, a3, 0xc0);
        RegisterDataMove(gUnknown_08485A2C, (u8 *)a3 + 0xc0, 0x40);
        break;
    case 10:
        RegisterDataMove(src + 0x140, a3, 0xc0);
        RegisterDataMove(gUnknown_08485A2C, (u8 *)a3 + 0xc0, 0x40);
        break;
    case 6:
        RegisterDataMove(src + 0x440, a3, 0xc0);
        RegisterDataMove(gUnknown_08485A2C, (u8 *)a3 + 0xc0, 0x40);
        break;
    case 14:
        RegisterDataMove(gUnknown_08485A2C, a3, 0x40);
        RegisterDataMove(src + 0x80, (u8 *)a3 + 0x40, 0x80);
        RegisterDataMove(gUnknown_08485A2C, (u8 *)a3 + 0xc0, 0x40);
        break;
    case 17:
        RegisterDataMove(src + 0xa40, a3, 0xc0);
        RegisterDataMove(gUnknown_08485A2C, (u8 *)a3 + 0xc0, 0x40);
        break;
    }
}
asm(".global sub_0803F6BC\n.thumb_set sub_0803F6BC, LoadTerrainObjTiles\n");
