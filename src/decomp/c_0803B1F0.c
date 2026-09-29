#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B1F0.
 * sub_0803B1F0 @ 0x0803B1F0
 */

/* Draws the two halves of a bobbing prompt: a fixed badge, and a strip whose y
 * comes from a ten-frame ramp advanced every third frame.
 *
 * `(u32)gGameClock / 3 % 10` and not a signed divide -- both helpers are
 * the UNSIGNED pair (__udivsi3 then __umodsi3), and gGameClock is
 * declared s32, so the cast is load-bearing.
 *
 * `movs r3,#0x80; lsls r3,#6` is the literal 0x2000. */
void sub_0803B1F0(void)
{
    PutOamHi(0x38, gUnknown_080910E8[(u32)gGameClock / 3 % 10] + 0x6c,
                 gUnknown_0849E6B8, 0x5054);
    PutOamHi(0x48, 0x88, gUnknown_0849E6A4, 0x2000);
}
