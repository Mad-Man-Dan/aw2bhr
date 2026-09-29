#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034C90.
 * sub_08034C90 @ 0x08034C90, sub_08034CA4 @ 0x08034CA4
 */

/* Family F077, second member -- see sub_0802E2BC. This is the function
 * unknown-globals.h names as the counter-example that proves 0x08090D88 is a
 * compiler pool word and not a global: it writes gUnknown_030032D8 by naming
 * the symbol directly, as here. */

void MapState_StartFuelUpkeep(void)
{
    sub_0802A7B0();
    gUnknown_030032D8 = 9;
}
asm(".global sub_08034C90\n.thumb_set sub_08034C90, MapState_StartFuelUpkeep\n");

/* Family F077, third member -- see sub_0802E2BC. */

void MapState_StartTurnStartSupply(void)
{
    sub_0802A538();
    gUnknown_030032D8 = 0xa;
}
asm(".global sub_08034CA4\n.thumb_set sub_08034CA4, MapState_StartTurnStartSupply\n");
