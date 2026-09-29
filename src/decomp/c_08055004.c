#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08055004.
 * sub_08055004 @ 0x08055004
 */

/* LoadInfantryEffectArt's third sibling and the `case 0x12` arm of sub_08054E8C: the
 * same shape as c_08054E8C.c's pair, one slot further into the same VRAM
 * block (+0x1400 instead of +0xA00), a different source blob, 0x2C8 words
 * instead of 0x288, and only ONE of the three parallel tables written.
 *
 * `off` has to be its own statement ahead of `v`, exactly as c_08054E8C.c
 * records: inlined into the CpuFastSet argument it becomes part of the call's
 * argument setup and is evaluated after the source pool word is loaded, which
 * is the wrong order.
 *
 * Both are u16: the ROM truncates each with an `lsls #0x10; lsrs #0x10` pair
 * that an int would not need. */
void LoadBCopterEffectArt(u16 a1, u16 a2)
{
    u16 v;

    if (a2 != 3)
    {
        u16 off = a1 * 0x2000 + 0x1400;

        v = a1 * 0x100 + 0xa0;
        CpuFastSet(gUnknown_0855089C, (void *)(0x06010000 + off), 0x2C8);
    }
    else
    {
        v = 0x80;
    }
    gUnknown_020296B0[a1].unk00 = v;
}
asm(".global sub_08055004\n.thumb_set sub_08055004, LoadBCopterEffectArt\n");
