#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080261A4.
 * sub_080261A4 @ 0x080261A4
 */

/* A 2-D table lookup: the row is `GetPlayerCoCountry(a1) - 1` scaled by `muls #0x32`
 * (0x32 BYTES, i.e. 0x19 halfwords) and the column is a2 scaled by `lsls #1`.
 * gUnknown_08499608 is declared as `u16 [][0x19]` in wave 29 so both scales
 * come out of the type; hand-rolled arithmetic on a flat array would put the
 * row multiply in the wrong place.
 *
 * The result is shifted left by 2 and DesignRoomDrawUnitIcon masks it with 0x3ff, so the
 * entries are tile indices. `pop {r4}; pop {r1}` is the value-returning
 * epilogue and both parameters arrive bare, hence `int` for both. */

int GetUnitSpriteTile(int a1, int a2)
{
    return gUnknown_08499608[GetPlayerCoCountry(a1) - 1][a2] << 2;
}
asm(".global sub_080261A4\n.thumb_set sub_080261A4, GetUnitSpriteTile\n");
