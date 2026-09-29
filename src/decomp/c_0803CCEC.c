#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CCEC.
 * sub_0803CCEC @ 0x0803CCEC
 */

/* A member of the 0x0803Cxxx family only by address -- it is not a bit
 * accessor. It hands out the name string of slot `a`, substituting the ROM
 * fallback gUnknown_0849F320 when the slot is empty (unk13 == 0xff), which is
 * the same emptiness test LoadDesignRoomName makes before copying the name out.
 *
 * The parameter is `u8`: the `lsl #24; lsr #24` at entry is parameter
 * narrowing at the point of use, and an `int` index emits nothing there.
 * `adds r0, r2, #2` puts the member offset on the BASE rather than folding it
 * into the pool word, which is what an aggregate member offset always does --
 * see the pool-fold table in docs/agbcc-codegen.md.
 */
u8 *GetDesignRoomSlotName(u8 a)
{
    if (gUnknown_020280C0[a].unk13 == 0xff)
        return gUnknown_0849F320;

    return gUnknown_020280C0[a].unk02;
}
asm(".global sub_0803CCEC\n.thumb_set sub_0803CCEC, GetDesignRoomSlotName\n");
