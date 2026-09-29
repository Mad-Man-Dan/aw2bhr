#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080005FC.
 * sub_080005FC @ 0x080005FC
 */

/*
 * DesignRoomMode_Start -- wait 30 frames, then hand one of two tables to StartEventScript.
 *
 * On the first frame after the mode change (stateChanged set) it clears the
 * state and arms a 30-frame timer. While the state is 0 it counts the timer
 * down; at zero it moves to state 1 and calls StartEventScript with
 * gUnknown_084856FC if sub_08004E44 returned 0 and gUnknown_084857AC
 * otherwise. What sub_08004E44 tests, and what StartEventScript does with the
 * table, are not visible from here.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - The table must be chosen with `?:` inside the call. An if/else with a
 *     call in each arm leaves two call instructions in the output.
 */

void DesignRoomMode_Start(void)
{
    if (gActiveMap->stateChanged != 0)
    {
        gActiveMap->stateChanged = 0;
        gActiveMap->state = 0;
        gActiveMap->stateTimer = 30;
    }

    if (gActiveMap->state == 0)
    {
        if (--gActiveMap->stateTimer == 0)
        {
            gActiveMap->state = 1;
            StartEventScript(sub_08004E44() == 0 ? gUnknown_084856FC : gUnknown_084857AC);
        }
    }
}
asm(".global sub_080005FC\n.thumb_set sub_080005FC, DesignRoomMode_Start\n");
