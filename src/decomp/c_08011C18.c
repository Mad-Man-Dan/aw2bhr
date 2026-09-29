#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011C18.
 * sub_08011C18 @ 0x08011C18
 */

/* Plain `gArray[i].member` throughout: the two word members hoist their offset
 * onto the base (the +4 one as the loop-invariant `adds r5, r4, #4`) while the
 * halfword and byte members fold into displacements off the element address.
 * The dead `ldrh`/`ldrb` ahead of each of those two stores is the volatile
 * aggregate-member tell; the words carry no signal because SImode never gets
 * it. Counter is u16 -- `adds #1; lsls #16; lsrs #16` every iteration. */
void ClearTileRigistry(void)
{
    u16 i;

    gUnknown_03002F30 = 0;

    for (i = 0; i < 48; i++)
    {
        gUnknown_0200B3B4[i].unk00 = 0;
        gUnknown_0200B3B4[i].unk04 = 0;
        gUnknown_0200B3B4[i].unk08 = 0;
        gUnknown_0200B3B4[i].unk0a = 0;
    }
}
asm(".global sub_08011C18\n.thumb_set sub_08011C18, ClearTileRigistry\n");
