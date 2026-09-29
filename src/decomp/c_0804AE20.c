#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AE20.
 * sub_0804AE20 @ 0x0804AE20
 */

/* `lsls r0, #0x18; cmp r0, #0` on the result of sub_08019260 is the declared
 * `bool8` return being re-narrowed at the call site, not a cast in the source.
 *
 * `adds r1, #0x62` before the `ldrb`/`strb` pair is strb's 31-byte
 * displacement limit; unk62 is a plain u8 counter.
 *
 * gUnknown_03001FBC is s16 and ClearSlotScriptCallback takes u8, so the `ldrb` is the
 * prototype's conversion reading the low half. */
void NameEntry_WaitPromptStep(void)
{
    if (sub_08019260() == 0)
    {
        gUnknown_030044E0->unk62++;
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
asm(".global sub_0804AE20\n.thumb_set sub_0804AE20, NameEntry_WaitPromptStep\n");
