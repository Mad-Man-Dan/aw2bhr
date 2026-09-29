#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08021598.
 * sub_08021598 @ 0x08021598
 */

void InitNewMapState(void)
{
    ResetAllPlayers();
    InitPlayersFromSettings();
    AdvanceToNextActiveArmy();
    CalcRandomWeatherChances();
    SpawnInventionRecords();
    InitPipeSeamHpPlane();
}
asm(".global sub_08021598\n.thumb_set sub_08021598, InitNewMapState\n");
