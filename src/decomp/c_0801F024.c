#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F024.
 * sub_0801F024 @ 0x0801F024
 */

/* Run-or-defer. NOT written with `return` in either arm: a `return f(...)` in
 * both halves makes gcc lay the else-block out first and inverts the branch to
 * `bne`. Falling off the end leaves the callee's r0 in place, which is what the
 * ROM does and what the `int` return in the header describes. */
int RunOrQueueDrawCallback(void *f, u16 b)
{
    if (gUnknown_03001FE0)
        ((int (*)(u32, u16))f)(gUnknown_03001FE0, b);
    else
        QueueSpriteCallback(f, b);
}
asm(".global sub_0801F024\n.thumb_set sub_0801F024, RunOrQueueDrawCallback\n");
