#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048F10.
 * sub_08048F10 @ 0x08048F10
 */

/* A saturating step on the s16 at +0x832: while it is negative it climbs by 8,
 * and the first non-negative frame pins it to 0. The NEW value is returned, so
 * the caller sees the clamp in the same frame.
 *
 * The ldrsh is the whole signedness argument -- see the note on unk832 in
 * unknown-globals.h. The ldrh beside it is the += 8 reading the same word.
 *
 * gUnknown_084C30F8 is fetched again for the return because the strh may alias
 * the pointer itself. Both fetches go through a .rodata address-constant word
 * (-fforce-addr), which is why the ROM shows a double ldr and a
 * gUnknown_0812A154 pool symbol; that word holds &gUnknown_084C30F8 and the
 * split build places it. */
u16 ShopScreen_StepOffsetToZero(void)
{
    if (gUnknown_084C30F8->unk832 < 0)
        gUnknown_084C30F8->unk832 += 8;
    else
        gUnknown_084C30F8->unk832 = 0;

    return gUnknown_084C30F8->unk832;
}
asm(".global sub_08048F10\n.thumb_set sub_08048F10, ShopScreen_StepOffsetToZero\n");
