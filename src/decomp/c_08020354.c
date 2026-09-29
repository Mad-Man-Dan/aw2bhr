#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020354.
 * sub_08020354 @ 0x08020354, sub_080203C0 @ 0x080203C0
 */

/* Two overlay writes at (x, y) for one unit record: the unit's own current
 * value, then its type's gUnknown_085D5ABC[t].unk0e capacity minus one under a
 * different flag.
 *
 * `((e - gUnits) >> 6) + 1` is the standard army number for this
 * tree -- the *5 / *17 / *257 / *65537 shift-add chain plus `neg; asr #8` is
 * agbcc's exact division by the 12-byte stride folded together with the `>> 6`
 * that groups 64 slots to an army. */
void FillUnitAttackRange(u16 x, u16 y, struct Unit *e)
{
    MapSetInRange(x, y,
                 GetUnitFiringRangeWithCoBonus(((e - gUnits) >> 6) + 1, e->type), 0);
    MapSetInRange(x, y, gUnknown_085D5ABC[e->type].minRange - 1, 0xff);
}
asm(".global sub_08020354\n.thumb_set sub_08020354, FillUnitAttackRange\n");

/* Clears the four orthogonal neighbours of cell (x, y) in the
 * gUnknown_03003340 row-pointer overlay, each guarded by the matching edge
 * test: the low edges against 0, the high edges against the gUnknown_08499590
 * map header's u16 width (+0) and height (+2).
 *
 * The header pointer is read through `((u16 *)gUnknown_08499590)[0]` and `[1]`
 * rather than a struct: two reads of one global, which is the documented case
 * where agbcc keeps the SYMBOL address in a callee-saved register (r4) and
 * reloads the pointer, exactly as the ROM does. */
void MapZeroNeighbors(int x, int y)
{
    if (x > 0)
        gUnknown_03003340[y][x - 1] = 0;
    if (x <= gMap->width)
        gUnknown_03003340[y][x + 1] = 0;
    if (y > 0)
        gUnknown_03003340[y - 1][x] = 0;
    if (y <= gMap->height)
        gUnknown_03003340[y + 1][x] = 0;
}
asm(".global sub_080203C0\n.thumb_set sub_080203C0, MapZeroNeighbors\n");
