#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CD00.
 * sub_0802CD00 @ 0x0802CD00, sub_0802CD14 @ 0x0802CD14
 */

/* F018: `push {lr}; bl a; bl b; bl c; pop {r0}; bx r0`.
 * THREE INDEPENDENT STATEMENTS, not a nest. Nothing moves r0 between the `bl`s,
 * which by itself does not separate `c(b(a()))` from three calls -- both leave
 * r0 alone. What settles it is the callees: all three take no arguments (none
 * of them reads r0-r3 before writing it) and all three end `pop {r0}`, i.e.
 * void, so there is no value to nest. src/decomp/c_08048558.c is the matched
 * exemplar of the same shape. */
void MapMenu_Options(void)
{
    PushMenu();
    CloseTopMenu();
    OpenOptionsMenu();
}
asm(".global sub_0802CD00\n.thumb_set sub_0802CD00, MapMenu_Options\n");

/* F018: `push {lr}; bl a; bl b; bl c; pop {r0}; bx r0`.
 * THREE INDEPENDENT STATEMENTS, not a nest. Nothing moves r0 between the `bl`s,
 * which by itself does not separate `c(b(a()))` from three calls -- both leave
 * r0 alone. What settles it is the callees: all three take no arguments (none
 * of them reads r0-r3 before writing it) and all three end `pop {r0}`, i.e.
 * void, so there is no value to nest. src/decomp/c_08048558.c is the matched
 * exemplar of the same shape. */
void MapMenu_Intel(void)
{
    PushMenu();
    CloseTopMenu();
    OpenIntelMenu();
}
asm(".global sub_0802CD14\n.thumb_set sub_0802CD14, MapMenu_Intel\n");
