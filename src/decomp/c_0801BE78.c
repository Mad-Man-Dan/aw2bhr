#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BE78.
 * sub_0801BE78 @ 0x0801BE78
 */

/* Rebuilds the sprite-list free chain: 16 layer-head entries linked to their
 * successor with their object pointers cleared, then the last link and the
 * one at index 4 broken, then the bump allocator reset to the pool base.
 *
 * Four things are load-bearing and none of them is visible in the semantics:
 *
 *  - `pp` exists only to keep -fforce-addr from routing gUnknown_0808F090's
 *    address through .rodata.  The global is read before AND after the loop,
 *    which normally triggers it; taking its address into a local is the one
 *    escape a POINTER global has (the aggregate-member escape does not apply).
 *  - The two cursors are separate locals assigned in this order.  Written as
 *    `p[i].next = &p[i + 1]` the value cursor is created second and the entry
 *    cursor needs a copy (`adds rD, rB, #0`) because `adds rD, #0xf0` can only
 *    write its own operand.
 *  - `nul` gives the cleared object pointer a statement of its own, which is
 *    what puts its `movs #0` between the deref and the 0x100 constant rather
 *    than at the end of the preheader where loop-invariant motion would sink
 *    a bare NULL.
 *  - `r` rather than reusing `p` for the tail: reusing `p` splits its web and
 *    permutes the whole loop's allocation while emitting the same
 *    instructions.  This one is the difference between a match and a
 *    same-length register permutation. */
void ClearSprites(void)
{
    struct SpriteEntry **pp;
    struct SpriteEntry *p;
    struct SpriteEntry *q;
    u16 *nul;
    int i;
    struct SpriteEntry *r;

    i = 15;
    pp = &gUnknown_0808F090;
    p = gUnknown_0808F090;
    nul = NULL;
    q = p + 16;
    p += 15;
    do
    {
        p->next = q;
        p->object = nul;
        q--;
        p--;
        i--;
    } while (i >= 0);

    r = *pp;
    r[15].next = NULL;
    r[4].next = NULL;
    gUnknown_03002B24 = gUnknown_0200CD10;
}
asm(".global sub_0801BE78\n.thumb_set sub_0801BE78, ClearSprites\n");
