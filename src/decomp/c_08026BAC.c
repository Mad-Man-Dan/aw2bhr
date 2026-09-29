#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026BAC.
 * sub_08026BAC @ 0x08026BAC
 */

/* Clears the five army records and stamps each one's identity: unk2a gets the
 * slot index inside the loop, and unk2b gets that slot's single mask bit
 * (1, 2, 4, 8 for slots 1..4) in four statements afterwards. Slot 0 is reset
 * like the rest but gets no bit.
 *
 * `i` is an `int` -- the exit test is `cmp r4, #4; ble`, SIGNED, and the value
 * is stored into the u8 unk2a with no narrowing at all.
 *
 * The array pointer is reloaded before nearly every store because each store
 * may alias it; where consecutive fields DO share one `ldr` (unk00/unk04/
 * unk11, the unk14..unk1a run, unk20/unk24, unk26/unk28/unk2a) that is agbcc's
 * own CSE and not a different source spelling -- one plain assignment
 * statement per field reproduces the grouping exactly.
 *
 * gUnknown_08090A8C is the -fforce-addr word holding &gPlayers, which
 * is why the chain is three `ldr`s deep. Named honestly; the build places the
 * word. */
void ResetAllPlayers(void)
{
    int i;

    for (i = 0; i <= 4; i++)
    {
        gPlayers[i].funds = 0;
        gPlayers[i].spent = 0;
        gPlayers[i].captures = 0;
        gPlayers[i].unk12 = 0;
        gPlayers[i].unk13 = 0;
        gPlayers[i].defeated = 0;
        gPlayers[i].destroyedThisTurn = 0;
        gPlayers[i].totalDestroyed = 0;
        gPlayers[i].teamColor = 0;
        gPlayers[i].aiControlled = 0;
        gPlayers[i].turnState = 0;
        gPlayers[i].coMode = 0;
        gPlayers[i].coActivationMode = 0;
        gPlayers[i].coCharge = 0;
        gPlayers[i].unk24 = 0;
        gPlayers[i].unk25 = 0;
        gPlayers[i].tempFirepower = 0;
        gPlayers[i].tempDefense = 0;
        gPlayers[i].team = i;
        gPlayers[i].unk31 = 0;
        gPlayers[i].killOnEndTurn = 0;
        gPlayers[i].unitCount = 0;
        gPlayers[i].unitsLost = 0;
    }

    gPlayers[1].unk2b = 1;
    gPlayers[2].unk2b = 2;
    gPlayers[3].unk2b = 4;
    gPlayers[4].unk2b = 8;
}
asm(".global sub_08026BAC\n.thumb_set sub_08026BAC, ResetAllPlayers\n");
