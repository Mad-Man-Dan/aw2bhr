#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035124.
 * sub_08035124 @ 0x08035124, ChangeGameWeather @ 0x08035144
 */

void ResetWeatherRoundCountersIfChanging(u8 a)
{
    if (a != 0 && gPlaySt.weather != a)
        ResetWeatherRoundCounters();
}
asm(".global sub_08035124\n.thumb_set sub_08035124, ResetWeatherRoundCountersIfChanging\n");

/* Named per Xenesis's AW2 Subroutine List: "Changes current game weather.
 * r0 = Weather ID". The old ChangeGameWeather symbol is kept as a linker alias
 * below so every other unit keeps resolving it unchanged. */
void ChangeGameWeather(u8 a)
{
    ResetWeatherRoundCounters();
    gUnknown_03004490[0] = 0x32;
    sub_080152EC(gUnknown_0849BD38, 0)->unk20 = a;
}

asm(".global sub_08035144\n.thumb_set sub_08035144, ChangeGameWeather\n");
