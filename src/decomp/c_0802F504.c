#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F504.
 * sub_0802F504 @ 0x0802F504, sub_0802F534 @ 0x0802F534
 */

/* Counts how many of the four SioIsPlayerSending bits are set and returns the count
 * as s8.
 *
 * NOT a bitfield: `lsls r5,#0x18 / movs r1,#0x80 / lsls r1,#0x11 / adds /
 * lsrs r5,#0x18` is `n++` on an s8 LOCAL with combine folding the add into the
 * value's own sign-extension, so the increment happens in the high byte and
 * `0x80 << 0x11` is 1 << 24, i.e. plus ONE. Written plainly it reproduces. */
s8 SioCountSendingPlayers(void)
{
    s8 n;
    int i;

    n = 0;
    for (i = 0; i <= 3; i++)
    {
        if (SioIsPlayerSending(i) == 1)
            n++;
    }
    return n;
}
asm(".global sub_0802F504\n.thumb_set sub_0802F504, SioCountSendingPlayers\n");

/* SioCountSendingPlayers's twin over SioIsPlayerLinked's bit array, but the counter is the
 * record's own unk07 rather than a local, so every step is a real store.
 *
 * The discarded `ldrb` before each `strb` is the VOLATILE tell this repo has
 * settled five times over (unk00/unk01/unk04/unk07/unk08 all carry it) -- it is
 * NOT a bitfield read-modify-write. unk07 is already `volatile u8` in
 * include/unknown-globals.h from wave 33 and needed no change; plain `= 0` and
 * `++` reproduce both the single dead load and the doubled one.
 *
 * The `lsls #0x18; asrs #0x18` on the way out is the s8 return converting the
 * u8 member, not evidence the member is signed. */
s8 SioCountLinkedPlayers(void)
{
    int i;

    gUnknown_0849B018->unk07 = 0;

    for (i = 0; i <= 3; i++)
    {
        if (SioIsPlayerLinked(i) == 1)
            gUnknown_0849B018->unk07++;
    }

    return gUnknown_0849B018->unk07;
}
asm(".global sub_0802F534\n.thumb_set sub_0802F534, SioCountLinkedPlayers\n");
