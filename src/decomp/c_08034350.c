#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034350.
 * sub_08034350 @ 0x08034350
 */

/* A two-arm `switch`, not an if/else chain: the literal pool sits between the
 * dispatch and the two case bodies, which is where agbcc parks it for a switch
 * that falls out of the compare chain with a `b` to the epilogue. */
void MapState_RemoteTurn(void)
{
    gUnknown_030040DC = 0;

    switch (gUnknown_03003F60)
    {
    case 0:
        RemoteTurn_WaitForCommand();
        break;
    case 4:
        RemoteTurn_ExecuteCommand();
        break;
    }
}
asm(".global sub_08034350\n.thumb_set sub_08034350, MapState_RemoteTurn\n");
