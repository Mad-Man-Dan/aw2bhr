#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B06C.
 * sub_0804B06C @ 0x0804B06C, sub_0804B088 @ 0x0804B088
 */

/* `lsls r0, r0, #0x18; cmp r0, #0` is agbcc re-narrowing sub_08019260's
 * declared `bool8` return -- not a cast in the source. gUnknown_03001FBC is a
 * declared `s16` and ClearSlotScriptCallback takes `u8`, so the conversion is a bare
 * `ldrb` off the little-endian low byte rather than a load plus a mask. */
void NameEntry_WaitMessageStep(void)
{
    if (!sub_08019260())
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_0804B06C\n.thumb_set sub_0804B06C, NameEntry_WaitMessageStep\n");

/* gUnknown_03001FF8 is declared `volatile u16` and the zero is still shared
 * with the gUnknown_030030A0 store (one `movs r1, #0` feeding both `strh`s) --
 * volatile constrains the ACCESS, not the constant that feeds it. */
void NameEntry_OnEnd(void)
{
    gUnknown_03001FF8 = 0;
    gUnknown_030030A0 = 0;

    sub_0801537C(gUnknown_084C3D6C);
    sub_0801537C(gUnknown_084C3D8C);
}
asm(".global sub_0804B088\n.thumb_set sub_0804B088, NameEntry_OnEnd\n");
