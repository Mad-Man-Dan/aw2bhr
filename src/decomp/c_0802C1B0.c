#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C1B0.
 * sub_0802C1B0 @ 0x0802C1B0, sub_0802C1C0 @ 0x0802C1C0
 */

/* Installs one gUnknown_0200C528 list script. sub_080193B0 returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void sub_0802C1B0(void)
{
    sub_080193B0(gUnknown_0849A5E0);
}

/* Installs one gUnknown_0200C528 list script. sub_080193B0 returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void sub_0802C1C0(void)
{
    sub_080193B0(gUnknown_0849A6B0);
}
