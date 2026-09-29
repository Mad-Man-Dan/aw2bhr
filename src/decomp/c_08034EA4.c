#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034EA4.
 * sub_08034EA4 @ 0x08034EA4, sub_08034ED0 @ 0x08034ED0, sub_08034EF0 @ 0x08034EF0
 */

/* Wave 24's fall-through rule reads this straight off: the `bne` is the
 * negation of the source condition and the `if` body -- the 9 -- falls through.
 * The literal pool sitting between the two arms is why the else arm gets its
 * own copy of &gUnknown_030032D8. */
void MapState_StartDayStartScreen(void)
{
    StartDayStartScreen();

    if (gUnknown_03004080 == 1)
        gUnknown_030032D8 = 9;
    else
        gUnknown_030032D8 = 8;
}
asm(".global sub_08034EA4\n.thumb_set sub_08034EA4, MapState_StartDayStartScreen\n");

/* The other half of ParkMapState, which parks gUnknown_030032D8 in
 * gUnknown_030044DC and writes 0x10 in its place. */
void MapState_RunParkedWinLossCheck(void)
{
    gUnknown_030032D8 = gUnknown_030044DC;
    gUnknown_030044DC = 0;
    RunWinLossCheck();
}
asm(".global sub_08034ED0\n.thumb_set sub_08034ED0, MapState_RunParkedWinLossCheck\n");

/* The local is load-bearing: the ROM stores r4 -- the narrowed return value,
 * known to be zero on this path -- into gUnknown_030032D8 rather than a fresh
 * `movs r0, #0`. Writing the constant instead costs an extra instruction. */
void MapState_EndOfGame(void)
{
    u8 v = sub_08019260();

    if (v == 0)
    {
        StartEndOfGameProc();
        gUnknown_030032D8 = v;
    }
}
asm(".global sub_08034EF0\n.thumb_set sub_08034EF0, MapState_EndOfGame\n");
