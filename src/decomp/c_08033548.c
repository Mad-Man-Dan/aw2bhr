#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033548.
 * sub_08033548 @ 0x08033548
 */

/* Passes the ADDRESS of gGameClock -- `ldr r0, =gGameClock` with
 * no load through it. SioSend16 reads its argument with `ldrh` on the way to
 * SIOMLT_SEND, so the parameter is `u16 *` and the global is the `s32` the
 * header already fixed from its arithmetic users; the cast is byte-neutral. */
void LinkSendGameClockWord(void)
{
    SioSend16((u16 *)&gGameClock, -1);
}
asm(".global sub_08033548\n.thumb_set sub_08033548, LinkSendGameClockWord\n");
