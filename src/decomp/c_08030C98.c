#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08030C98.
 * sub_08030C98 @ 0x08030C98
 */

/* Arm the link session: two timeouts of 0x66 frames, mode 1, then hand the
 * descriptor to SioRegisterIrq and reset the state machine.
 *
 * The DEAD `ldrb [r2, #1]` immediately before the `strb` is the whole reason
 * unk01 is volatile -- see the note in unknown-globals.h. The same dead-read
 * shape appears again at the tail for unk04, which was already volatile.
 *
 * SioRegisterIrq's argument costs nothing: r0 still holds &gUnknown_030040C0 from
 * the two stores above it. */
void LinkStartHandshake(void)
{
    gUnknown_030040C0.unk06 = 0x66;
    gUnknown_030040C0.unk0a = 0x66;
    gUnknown_0849B018->unk01 = 1;
    SioRegisterIrq(&gUnknown_030040C0);
    SioResetPollingState();
    gUnknown_0849B018->unk04 = 3;
}
asm(".global sub_08030C98\n.thumb_set sub_08030C98, LinkStartHandshake\n");
