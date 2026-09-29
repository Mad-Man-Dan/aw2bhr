#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080581A4.
 * sub_080581A4 @ 0x080581A4
 */

/* Wave 52, W52-B.  MATCHED, 2 attempts.
 *
 * Seeds a whole map plane to one byte value through a stack table of 40 row
 * pointers (`sub sp,#0xa0`), built from gUnknown_08499590's +0x417A rowOffset
 * table and then walked row by row.
 *
 * PROMOTION MUST CARRY  "rodata": ["0x0816D938"] .  That ROM word holds
 * 0x08499590, i.e. &gUnknown_08499590 -- agbcc's own -fforce-addr address
 * constant for this function, which is why the symbol is reached with a
 * three-level `ldr rA,=<rodata slot>; ldr rB,[rA]; ldr rC,[rB]`.  The honest
 * `gUnknown_08499590` spelling reproduces it and trymatch reports `relocs:
 * name different symbols that resolve to the same address`.
 *
 * The map header MUST be reached through a locally declared struct cast onto
 * the `u8 *` symbol, not as pointer arithmetic -- wave 34 (W34-F)'s rule,
 * recorded on gUnknown_08499590 in include/unknown-globals.h.  Written as
 * `*(u16 *)(gUnknown_08499590 + 2)` the height reassociates, becomes
 * loop-invariant, `check_dbra_loop` reverses the first loop into a countdown,
 * and the ROM's re-read of `->unk02` at the loop bottom disappears (-4 bytes).
 */

void FillMapPlane(u8 *dst, int a2)
{
    u8 *rows[40];
    u8 v;
    int i;
    int j;

    v = a2;

    for (i = 0; i < gMap->height; i++)
        rows[i] = dst + gMap->rowOffset[i];

    for (i = 0; i < gMap->height; i++)
    {
        for (j = 0; j < gMap->width; j++)
            rows[i][j] = v;
    }
}
asm(".global sub_080581A4\n.thumb_set sub_080581A4, FillMapPlane\n");
