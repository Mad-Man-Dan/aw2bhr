#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041E94.
 * sub_08041E94 @ 0x08041E94
 */

/* Family F031. sub_080743E8's parameter is `struct Unk030040D8 *` and not the
 * `int` a pass-through would suggest: its other caller CommitUnitMove reaches it
 * the same way, `ldr r0,[r4]` off &gUnknown_030040D8. `pop {r0}` discards the
 * callee's u8 result, so this is void. */

void sub_08041E94(void)
{
    sub_080743E8(gUnknown_030040D8);
}
