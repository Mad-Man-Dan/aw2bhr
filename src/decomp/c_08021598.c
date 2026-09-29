#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08021598.
 * sub_08021598 @ 0x08021598
 */

void sub_08021598(void)
{
    ResetAllPlayers();
    InitPlayersFromSettings();
    AdvanceToNextActiveArmy();
    CalcRandomWeatherChances();
    SpawnInventionRecords();
    InitPipeSeamHpPlane();
}
