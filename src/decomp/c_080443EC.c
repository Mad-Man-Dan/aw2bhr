#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080443EC.
 * sub_080443EC @ 0x080443EC
 */

void sub_080443EC(void)
{
    sub_0803B4DC(0x23);
    ChangeGameWeather(1);
    sub_08039F58();
}
