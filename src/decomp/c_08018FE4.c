#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018FE4.
 * sub_08018FE4 @ 0x08018FE4
 */

/* A gUnknown_0200C528 list-script handler returning TRUE when it did nothing.
 * The node's unk08 is read BEFORE the test and used only on the else side, so
 * it is a local initialised at the top rather than an expression inside the
 * branch -- the ROM computes it unconditionally.
 *
 * The `!= 1` arm is the THEN arm: `cmp #1; beq` jumps to the lone `movs r3, #1`
 * and the call falls through. Writing the test the other way round produces the
 * same instructions in the opposite order and is 4 bytes longer.
 *
 * `int x`, not `s16`: the load is `ldrsh`, i.e. an `(s16)` cast on the `u16`
 * member reaching an int context, and PlayMusicAfterFade's parameter is word-width
 * (its prologue is a bare `adds r4, r0, #0`), so nothing re-narrows it. */
bool8 EventOp_PlayMusic(s16 a)
{
    bool8 r;
    int x;

    x = (s16)gUnknown_0200C528[a].unk04->unk08;
    if (gUnknown_03002514 != 1)
    {
        PlayMusicAfterFade(x);
        r = FALSE;
    }
    else
        r = TRUE;

    gUnknown_0200C528[a].unk04++;
    return r;
}
asm(".global sub_08018FE4\n.thumb_set sub_08018FE4, EventOp_PlayMusic\n");
