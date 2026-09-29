#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080351F0.
 * sub_080351F0 @ 0x080351F0
 */

void StartPendingWeatherChange(void)
{
    ResetWeatherRoundCountersIfChanging(gPlaySt.unk2e);

    if (gPlaySt.weather != gPlaySt.unk2e)
        sub_080152EC(gUnknown_0849BD38, 0)->unk20 = gPlaySt.unk2e;
}
asm(".global sub_080351F0\n.thumb_set sub_080351F0, StartPendingWeatherChange\n");
