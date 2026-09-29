#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AE50.
 * sub_0804AE50 @ 0x0804AE50
 */

/* Same guard as NameEntry_WaitPromptStep -- the `lsls #0x18` is sub_08019260's `bool8`
 * return being re-narrowed.
 *
 * The `(s32)` cast on gUnknown_084C3D7C is the price of the declared
 * `void sub_080152C0(s32, u8)` and emits nothing; every other matched caller
 * of sub_080152C0 in src/decomp carries the identical cast. */
void NameEntry_StartSlideInWhenIdle(void)
{
    if (sub_08019260() == 0)
    {
        sub_080152C0((s32)gUnknown_084C3D7C, 0);
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_0804AE50\n.thumb_set sub_0804AE50, NameEntry_StartSlideInWhenIdle\n");
