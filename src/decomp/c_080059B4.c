#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080059B4.
 * sub_080059B4 @ 0x080059B4
 */

/* sub_08005838's shape with only the `!= 2` arm -- same three-parameter
 * signature and the same u8 third parameter. */
void sub_080059B4(int a1, int a2, u8 a3)
{
    if (a3 != 2)
    {
        gActiveMap->state = 0;
        PushMenu();
        CloseTopMenu();
        sub_080152C0((s32)gUnknown_0848863C, 0);
    }
}
