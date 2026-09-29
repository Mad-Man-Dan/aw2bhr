#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080215FC.
 * sub_080215FC @ 0x080215FC
 */

/* Fills gMap->rowOffset[y] = y * gMap->width for every row.
 *
 * The loop is written as an explicit entry test plus a do/while because that is
 * what the codegen says: the entry test loads the global (`ldr r2,=g; ldr r0,[r2]`)
 * while the bottom test re-reads the height through the body's local `map`
 * (`ldrh r2,[r2,#2]`). A plain `for` whose condition names the global reloads it
 * at the bottom -- the `strh` kills the cached pointer load -- which is one
 * instruction too many. */
void InitMapRowOffsets(void)
{
    struct Map *map;
    u8 i;

    i = 0;

    if (i < gMap->height)
    {
        do
        {
            map = gMap;
            map->rowOffset[i] = i * map->width;
            i++;
        }
        while (i < map->height);
    }
}
asm(".global sub_080215FC\n.thumb_set sub_080215FC, InitMapRowOffsets\n");
