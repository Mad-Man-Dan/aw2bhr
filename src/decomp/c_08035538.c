#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035538.
 * sub_08035538 @ 0x08035538, sub_08035548 @ 0x08035548, sub_08035558 @ 0x08035558
 */

void SetWeatherModeRandom(void)
{
    gPlaySt.randomWeatherOn = 1;
}
asm(".global sub_08035538\n.thumb_set sub_08035538, SetWeatherModeRandom\n");

void SetWeatherModeLocked(void)
{
    gPlaySt.randomWeatherOn = 2;
}
asm(".global sub_08035548\n.thumb_set sub_08035548, SetWeatherModeLocked\n");

void SetWeatherModeOff(void)
{
    gPlaySt.randomWeatherOn = 0;
}
asm(".global sub_08035558\n.thumb_set sub_08035558, SetWeatherModeOff\n");
