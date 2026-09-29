#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B0EC.
 * sub_0803B0EC @ 0x0803B0EC, sub_0803B118 @ 0x0803B118
 */

#include "hardware.h"

/* A per-frame hook: run sub_080116E8, then on L (0x200 in gpKeySt->held, the
 * same slot and the same test as sub_0803B1CC's 0x100/R next door) start the
 * gUnknown_0849E610 proc through StartDebugEditMenu and raise gUnknown_03002F1C.
 *
 * `movs r0,#0x80; lsls r0,r0,#2` is a PLAIN constant 0x200 and not wave 23's
 * named-constant-local shape: the shift is minimal for that value (0x200 needs
 * an imm8 of 0x80, since 0x100 does not fit in eight bits) and the `movs` and
 * the `lsls` write the SAME register. Both tells are absent.
 * `pop {r0}; bx r0` -> void. */

void DebugEntry_Init(void)
{
    sub_080116E8();

    if (gpKeySt->held & L_BUTTON)
    {
        StartDebugEditMenu();
        gUnknown_03002F1C = 1;
    }
}
asm(".global sub_0803B0EC\n.thumb_set sub_0803B0EC, DebugEntry_Init\n");

/* The parameter is `struct Unk03001470 *` and that is forced rather than
 * chosen: the else arm forwards the incoming register UNCHANGED
 * (`adds r0, r2, #0`) to sub_080153B8, whose first parameter is already
 * declared `struct Unk03001470 *` in include/unknown-functions.h.
 *
 * `if (++a->unk1e > 0x5a)` and not `a->unk1e++`: the ROM re-uses the register
 * the `strh` just wrote, sign-extends it and compares, which is the value of
 * the INCREMENTED object. The `lsls #0x10; asrs #0x10` on that value is what
 * types unk1e `s16` -- see the member comment in include/unknown-globals.h.
 * The `ldrb` on the `s16` global gUnknown_03001FBC is the s16 -> u8 conversion
 * for ClearSlotScriptCallback's declared `u8` parameter, folded into the load.
 * `pop {r0}; bx r0` -> void. */

void DebugEntry_Loop(struct Unk03001470 *a)
{
    if (gpKeySt->held & 4)
    {
        if (++a->unk1e > 0x5a)
        {
            StartDebugBackupUtility();
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
    else
    {
        sub_080153B8(a);
    }
}
asm(".global sub_0803B118\n.thumb_set sub_0803B118, DebugEntry_Loop\n");
