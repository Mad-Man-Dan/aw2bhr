#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028154.
 * sub_08028154 @ 0x08028154
 */

/* F018: `push {lr}; bl a; bl b; bl c; pop {r0}; bx r0`.
 * THREE INDEPENDENT STATEMENTS, not a nest. Nothing moves r0 between the `bl`s,
 * which by itself does not separate `c(b(a()))` from three calls -- both leave
 * r0 alone. What settles it is the callees: all three take no arguments (none
 * of them reads r0-r3 before writing it) and all three end `pop {r0}`, i.e.
 * void, so there is no value to nest. src/decomp/c_08048558.c is the matched
 * exemplar of the same shape. */
void DayStartScreen_Finish(void)
{
    ClearBg0Tilemap();
    BG_EnableSyncBG0();
    SetMapLayersDefault();
}
asm(".global sub_08028154\n.thumb_set sub_08028154, DayStartScreen_Finish\n");
