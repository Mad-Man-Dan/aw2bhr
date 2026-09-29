#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08004E44.
 * sub_08004E44 @ 0x08004E44
 */

/* The condition is spelt in the NEGATIVE, and that is what the branch layout
 * says. Both spellings are 68 bytes and both use three `beq` to a shared block,
 * but which value falls through differs: `if (A==1 || B==1 || C==1) return 0;
 * return 2;` puts `movs r0, #0` in the fall-through and `movs r0, #2`
 * out of line, and the ROM has it the other way round. Writing the chain as
 * `!= 1` with `&&` and returning 2 from the fall-through is the spelling that
 * matches -- the wave-24 "which arm falls through is decided by the source
 * condition's sense" rule, on a three-term short-circuit.
 *
 * `pop {r1}; bx r1` makes this value-returning; every caller discards it, so
 * `int` is the weakest fit. The `lsls #0x18; lsrs #0x18` before each `cmp` is
 * agbcc re-narrowing LoadDesignRoomName's declared bool8. */
int sub_08004E44(void)
{
    if (LoadDesignRoomName(0, gDesignRoomName) != 1
     && LoadDesignRoomName(1, gDesignRoomName) != 1
     && LoadDesignRoomName(2, gDesignRoomName) != 1)
        return 2;

    return 0;
}
