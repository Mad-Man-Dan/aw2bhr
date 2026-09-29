#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020634.
 * sub_08020634 @ 0x08020634
 */

/* Reverses the byte string [dst, end) in place through a 20-byte stack buffer
 * and terminates it with the sentinel 4 -- the same terminator ScanMovementScriptToEnd
 * just below stops on.
 *
 * The copy-back loop is peeled by hand in the source: the first byte is stored
 * before the loop and the loop test reads `*dst` back.  That read is what the
 * `lsls #0x18; lsrs #0x18` pair is -- global CSE forwards the just-stored
 * QImode value from both predecessors into the shared test block and has to
 * re-extend it there.  Testing a `u8` local instead folds the extension into
 * the `ldrb` and lets cross-jumping sink the `strb` into the test block, which
 * is 4 bytes short and one instruction out of place.
 *
 * `i++` must be its own statement after the store: written as `buf[i++]` the
 * `movs r4, #1` schedules ahead of the `strb` and cross-jumping fires again. */
void RevertMovementScript(u8 *dst, u8 *end)
{
    u8 buf[0x14];
    int i;

    i = 0;
    while (end != dst)
    {
        buf[i] = *--end;
        i++;
    }
    buf[i] = 4;
    i = 0;
    *dst = buf[i];
    i++;
    while (*dst != 4)
    {
        dst++;
        *dst = buf[i];
        i++;
    }
}
asm(".global sub_08020634\n.thumb_set sub_08020634, RevertMovementScript\n");
