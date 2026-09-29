#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806BD6C.
 * sub_0806BD6C @ 0x0806BD6C
 */

/* The first parameter is a plain `u16 *`, NOT a struct pointer, and the ROM
 * says which: through a struct member at +0x40 agbcc emits the value's
 * `adds r1, #1` BEFORE the address's `adds r0, #0x40`; through `p[0x20]` it
 * emits them in the ROM's order. (Binding `&p->unk40` to a local instead costs
 * an extra `adds r2, r0, #0` and is further away.) 0x20 * 2 == 0x40, which is
 * past `strh`'s 5-bit displacement, so the add is addressing rather than a
 * second object.
 *
 * `lsls #0x10; lsrs #0x10` at entry is PROMOTE_MODE on a `u16` parameter --
 * zero-extending, so the signedness is not settled by it, but the value is
 * only ever stored back as a halfword. */
void WriteBg0TilePair(u16 *p, u16 v)
{
    p[0] = v;
    p[0x20] = v + 1;
    BG_EnableSyncBG0();
}
asm(".global sub_0806BD6C\n.thumb_set sub_0806BD6C, WriteBg0TilePair\n");
