#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013434.
 * sub_08013434 @ 0x08013434
 */

#include "hardware.h"

/* Key-subsystem init: sets the repeat timing pair and clears all five
 * `struct KeySt` slots of the array based at gUnknown_03002040, then points
 * gpKeySt at slot 4 -- which is gUnknown_03002090, the buffer RefreshKeyStFromKeys
 * refreshes (see include/unknown-globals.h).
 *
 * The 20-byte stride is `struct KeySt`'s: the ROM synthesises it as
 * `((i * 4) + i) * 4`. Spelling the base as a cast subscript INSIDE the loop
 * rather than binding a `struct KeySt *` before it is load-bearing -- the
 * binding would put the pool `ldr` ahead of the `i = 0` init, where the ROM
 * has it after (the pool word is an LICM hoist, not a source statement).
 * The nine `strh`s are in the ROM's store order, which is the source order.
 * gpKeySt takes gUnknown_03002090's own pool word, not gUnknown_03002040+0x50. */
void InitKeySt(void)
{
    s16 i;

    gUnknown_030030C8 = 10;
    gUnknown_03002F94 = 4;

    for (i = 0; i <= 4; i++)
    {
        ((struct KeySt *)&gUnknown_03002040)[i].previous = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].held = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].pressed = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].unk10 = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].repeated = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].unk0e = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].unk08 = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].unk0c = 0;
        ((struct KeySt *)&gUnknown_03002040)[i].unk0a = 0;
    }

    gpKeySt = (struct KeySt *)&gUnknown_03002090;
}
asm(".global sub_08013434\n.thumb_set sub_08013434, InitKeySt\n");
