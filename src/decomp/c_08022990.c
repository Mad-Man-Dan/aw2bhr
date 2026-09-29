#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022990.
 * sub_08022990 @ 0x08022990
 */

/* The DrawRangeOverlayCellAt member of the 76-byte twin family (RedrawUnitLayer /
 * RedrawUnitIconLayer), wrapped in a setup/teardown: the sweep is 11 rows of 16
 * instead of 16 of 16 (`cmp r5,#0xa` at the outer bottom against `#0xf` at the
 * inner).
 *
 * gUnknown_080909B4 in the asm is NOT a global: the ROM word at 0x080909B4
 * holds 0x08499590, agbcc's own -fforce-addr address constant for
 * gUnknown_08499590 -- the fifth of that set (see include/unknown-globals.h).
 *
 * The first two parameters are dead here and cost no instruction: they are
 * already in r0/r1 at entry and ClearBg0Tilemap is declared nullary, so the
 * forwarding is invisible either way.  Only the third is narrowed at entry
 * (`lsls #0x10; lsrs #0x10`) and parked in r8 across the loop for the final
 * `strh` into the slot sub_080152EC hands back. */
void ShowRangeOverlay(int a1, int a2, u16 a3)
{
    u16 x;
    u16 y;

    ClearBg0Tilemap();

    for (y = 0; y <= 0xa; y++)
    {
        for (x = 0; x <= 0xf; x++)
        {
            DrawRangeOverlayCellAt((u16)(x + (gMap->scrollX >> 4)),
                         (u16)(y + (gMap->scrollY >> 4)));
        }
    }

    BG_EnableSyncBG0();
    gUnknown_03000559 = 1;
    sub_080152EC(gUnknown_08499B4C, 0xff)->unk1e = a3;
}
asm(".global sub_08022990\n.thumb_set sub_08022990, ShowRangeOverlay\n");
