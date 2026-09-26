#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017A58.
 * sub_08017A58 @ 0x08017A58
 */

/* `s16` and NOT `bool8`, settled from its caller: sub_08017D30 returns this
 * function's result unconverted and the ROM narrows it `lsls #0x10; asrs
 * #0x10`, where a `bool8` return probes as `lsls #0x18; lsrs #0x18`. The body
 * below is byte-identical either way -- `return FALSE` is `movs r0, #0` in both
 * -- so the call site is the only evidence there is. See the family note in
 * include/unknown-functions.h. Wave 26, W26-C. */
s16 sub_08017A58(s16 a)
{
    gUnknown_0200C528[a].unk0c = gUnknown_0200C528[a].unk04->unk0c;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
