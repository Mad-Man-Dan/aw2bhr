#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080227F4.
 * sub_080227F4 @ 0x080227F4
 */

/* Writes one 2x2 tile quad into the gBG0TilemapBuffer tilemap for map cell
 * (x, y): the four halfwords 0x81b0..0x81b3, or four zeroes when the cell's
 * gUnknown_03003340 byte is negative. The screen index is the promoted
 * `(x - camera_x) & 0xF` / `(y - camera_y) & 0xF` pair off gUnknown_08499590's
 * +0x0c / +0x0e, scaled to u16 elements (2 across, 64 down).
 *
 * The last of the four stores is SHARED by both arms -- agbcc falls the zero
 * arm into it -- and the 0x81b2 / 0x81b3 constants are reached by incrementing
 * the 0x81b1 pool word rather than pooled themselves, which is agbcc's own
 * constant CSE and not a different expression. */
void DrawRangeOverlayCellAt(u16 x, u16 y)
{
    u16 *p = gBG0TilemapBuffer
           + ((x - gMap->camX) & 0xF) * 2
           + ((y - gMap->camY) & 0xF) * 64;

    if ((s8)gUnknown_03003340[y][x] < 0)
    {
        p[0] = 0;
        p[1] = 0;
        p[32] = 0;
        p[33] = 0;
    }
    else
    {
        p[0] = 0x81B0;
        p[1] = 0x81B1;
        p[32] = 0x81B2;
        p[33] = 0x81B3;
    }
}
asm(".global sub_080227F4\n.thumb_set sub_080227F4, DrawRangeOverlayCellAt\n");
