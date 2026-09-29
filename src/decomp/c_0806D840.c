#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D840.
 * sub_0806D840 @ 0x0806D840
 */

/* The removal half of a gUnknown_03001470 list pair: sub_0806D820 installs
 * gUnknown_08581F40 through sub_080152EC(script, 3) and this drops it
 * through ClearCallbackOfSlotsRunningScript, which scans the 30 slots for the one whose .unk00
 * matches and ends it. `pop {r0}; bx r0`, so void.
 */

void sub_0806D840(void)
{
    ClearCallbackOfSlotsRunningScript(gUnknown_08581F40);
}
