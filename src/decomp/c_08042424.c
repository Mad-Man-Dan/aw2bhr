#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08042424.
 * sub_08042424 @ 0x08042424
 */

/* The c_0804247C.c map-cell idiom over the +0x1432 terrain plane: true when the
 * cell belongs to the gUnknown_03004084 army (top three bits) and its low five
 * bits are 0xa, 0xb or 0xe.
 *
 * DO NOT BIND THE LOADED CELL TO A LOCAL. Wave 35 parked this at 96.6% -- size
 * exact, relocs exact, three bytes differing, all from one register: the ROM
 * loads into the register that held the address (`ldrb r3,[r3]`, then
 * `ands r0,r3` twice) and a `u8 cell = cells[off];` local loads into r2
 * instead. Writing `cells[off]` at BOTH use sites removes the local, CSE keeps
 * the single `ldrb` anyway, and the load coalesces onto the dying address
 * register -- which is the ROM.
 *
 * This is worth generalising: the parked note ruled out declaration order,
 * binding the final address to its own local, destructive `p += 0x1432`, and
 * nesting the guard, and read the residual as "a pure register-allocation
 * residual with instruction order already correct". It was not an allocation
 * problem at all -- it was one local too many. The wave-17 rule "binding locals
 * are punctuation, not storage -- one per statement" applies to a VALUE local
 * as well as to an address local: a local whose only job is to name a value
 * read once extends that value's live range past the address's death and
 * blocks the coalesce.
 *
 * The `blt`/`ble`/`bne` chain rather than a jump table is agbcc's sparse-switch
 * form; the compares are signed, so `cell & 0x1f` is an int. */
u8 IsOwnFactoryCell(s16 x, s16 y)
{
    int off;

    off = gMap->rowOffset[y] + x;

    if ((gMap->terrain[off] & 0xe0) != gUnknown_03004084)
        return FALSE;

    switch (gMap->terrain[off] & 0x1f)
    {
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
        return TRUE;
    default:
        return FALSE;
    }
}
asm(".global sub_08042424\n.thumb_set sub_08042424, IsOwnFactoryCell\n");
