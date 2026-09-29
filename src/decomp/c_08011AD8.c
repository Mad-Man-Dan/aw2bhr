#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011AD8.
 * sub_08011AD8 @ 0x08011AD8
 */

typedef void (*Func8011AD8)(void);

/* Runs every callback registered on the 16-slot gUnknown_03002FA0 list by
 * QueueVBlankCallback, then clears the list with ClearVBlankCallbackQueue -- a one-shot
 * "deferred work" queue.
 *
 * gUnknown_030030E8 is the volatile count, read once for the zero-trip guard
 * (`ldrh; lsls #0x10; cmp #0; ble` -- the signed compare in the shifted
 * domain, so the asr folds away) and again at the bottom of every iteration,
 * which is what `volatile` buys.
 *
 * The cast goes on the ELEMENT, not on the array: `((Func)g[i])()` puts
 * gUnknown_03002FA0's address in the loop preheader and gUnknown_030030E8's
 * first in the pool, exactly as the ROM has it. Casting the array instead --
 * `((Func *)g)[i]()` -- creates that pseudo before the guard, swaps the two
 * pool words and pushes gUnknown_030030E8's address into a -fforce-addr
 * .rodata word reached through one extra `ldr`. */
void RunVBlankCallbackQueue(void)
{
    u8 i;

    for (i = 0; i < (s16)gUnknown_030030E8; i++)
        ((Func8011AD8)gUnknown_03002FA0[i])();

    ClearVBlankCallbackQueue();
}
asm(".global sub_08011AD8\n.thumb_set sub_08011AD8, RunVBlankCallbackQueue\n");
