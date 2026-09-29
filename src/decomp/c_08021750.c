#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08021750.
 * sub_08021750 @ 0x08021750
 */

/* Rebuilds the map's terrain plane from its tile-id table: for every cell,
 * terrain[idx] = gUnknown_0849959C[tile[idx]], where idx = rowOffset[y] + x.
 *
 * Two authoring points the diff insisted on:
 *  - The RHS lookup is its own statement, evaluated BEFORE the store address.
 *    Spelled as one assignment, agbcc forms &plane[idx] first, which makes
 *    0x1432's pseudo the older one; it then wins the callee-saved register and
 *    0x0A22 is rematerialised inside the loop.  The ROM has it the other way
 *    round (0x0A22 in ip, `ldr r0,=0x1432` in the loop body), which is the
 *    tile[] reference having been created first.
 *  - The seven zeroed fields at +4..+0x10 are seven separate assignments, one
 *    `strh` each -- an array plus a loop would not unroll.
 *
 * Uses the canonical gMap struct fields directly; this keeps the member
 * references that prevent agbcc from folding the plane constants into load
 * displacements.
 *
 * The parameter is an int, not a pointer: `adds r0,#0x4c` runs on it before the
 * `lsls #0x18; lsrs #0x18`, and that truncation is the conversion to
 * LoadSavedMapRecordWithUnits's already-promoted `u8` first parameter. */
void LoadSavedMapIntoGMap(int a)
{
    int x;
    int y;
    u16 t;

    LoadSavedMapRecordWithUnits((u8)(a + 0x4c), (int)gMap->unk421a);

    gMap->scrollX = 0;
    gMap->scrollY = 0;
    gMap->unk08 = 0;
    gMap->unk0a = 0;
    gMap->camX = 0;
    gMap->camY = 0;
    gMap->unk10 = 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            t = gMap->tile[gMap->rowOffset[y] + x];
            gMap->terrain[gMap->rowOffset[y] + x] = gUnknown_0849959C[t];
        }
    }
}
asm(".global sub_08021750\n.thumb_set sub_08021750, LoadSavedMapIntoGMap\n");
