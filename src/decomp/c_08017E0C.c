#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017E0C.
 * sub_08017E0C @ 0x08017E0C
 */

/* One step of the gUnknown_0200C528[i] script cursor, the same walker
 * src/decomp/c_08017DA0.c drives: publish the node's +0x08 and +0x0c into the
 * global and the slot, advance the cursor, and report whether the step was
 * SKIPPED because gUnknown_03002514 says a transition is running.
 *
 * The `!= 1` polarity is the readout: the ROM's `beq` sends the `== 1` case out
 * of line past the literal pool and runs the publish inline.
 *
 * gUnknown_0808E560 is not a global -- it is agbcc's `-fforce-addr` word for
 * &gUnknown_0200C528 (baserom.gba holds 0x0200C528 there), which is why both
 * pool words relocate against .rodata. The 24-byte stride and the +0x0c
 * halfword are struct Unk0200C528's own.
 *
 * PROMOTION NEEDS THE POOL WORD PLACED: this function's data/promoted.json
 * entry needs  "rodata": ["0x0808E560"]  and then tools/split_rodata.py +
 * tools/gen_lds.py before the build. */
int EventOp_HoldScriptedKeys(u16 i)
{
    int r;

    if (gUnknown_03002514 != 1)
    {
        gUnknown_03002EF0 = gUnknown_0200C528[(s16)i].unk04->unk08;
        gUnknown_0200C528[(s16)i].unk0c = gUnknown_0200C528[(s16)i].unk04->unk0c;
        r = 0;
    }
    else
    {
        r = 1;
    }

    gUnknown_0200C528[(s16)i].unk04++;

    return r;
}
asm(".global sub_08017E0C\n.thumb_set sub_08017E0C, EventOp_HoldScriptedKeys\n");
