#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065700.
 * sub_08065700 @ 0x08065700
 */

/* The removal half of a gUnknown_03001470 list pair: sub_080656E0 installs
 * gUnknown_08580C7C through sub_080152EC(script, 3) and this drops it
 * through ClearCallbackOfSlotsRunningScript, which scans the 30 slots for the one whose .unk00
 * matches and ends it. `pop {r0}; bx r0`, so void.
 */

void sub_08065700(void)
{
    ClearCallbackOfSlotsRunningScript(gUnknown_08580C7C);
}
