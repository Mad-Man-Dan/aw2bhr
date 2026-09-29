#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072BA4.
 * sub_08072BA4 @ 0x08072BA4
 */

/* Schedules PlaySeFunc to run after a delay: CallDelayedArg's three u32
 * parameters are (callback, argument, frames), and the callback reaches the
 * proc's +0x2c slot that CallDelayedArg_OnLoop calls through `_call_via_r1`.
 *
 * The cast is what makes the pool word a RELOCATION against PlaySeFunc
 * rather than a bare constant -- naming the function is the honest spelling
 * even though CallDelayedArg's promoted signature takes u32. */
void PlaySeDelayed(u32 a, u32 b)
{
    CallDelayedArg((u32)PlaySeFunc, a, b);
}
asm(".global sub_08072BA4\n.thumb_set sub_08072BA4, PlaySeDelayed\n");
