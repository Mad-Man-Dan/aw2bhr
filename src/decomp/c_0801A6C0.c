#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A6C0.
 * sub_0801A6C0 @ 0x0801A6C0
 */

/*
 * ResetSpriteRequestQueue -- clear the table gUnknown_0808E5C8 points at.
 *
 * .unk00 of all 0x81 entries goes to 0, then gUnknown_030020A8's first two
 * members and the table's own .unk04 are cleared.
 *
 * The function is declared as returning bool8 and has no return statement. The
 * original never sets the result register either, and its three callers all
 * throw the result away.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `pp` holds &gUnknown_0808E5C8 and the tail reaches the table through
 *     `*pp`. Naming the global both inside the loop and after it makes the
 *     compiler fetch its address from a constant in read-only data instead,
 *     which the original does not do.
 *   - The stored zero is the local `v`, and `asm("" : : "r"(v));` after the loop
 *     emits no instruction but keeps `v` alive past it. That is what makes the
 *     compiler put the table pointer and the zero in the same two registers the
 *     original uses; without it the two swap.
 */
bool8 ResetSpriteRequestQueue(void)
{
    struct Unk0808E5C8 **pp;
    struct Unk0808E5C8 *p;
    s16 i;
    u32 v;

    i = 0;
    pp = &gUnknown_0808E5C8;
    p = *pp;
    v = 0;

    for (; i <= 0x80; i++)
        p[i].unk00 = v;

    asm("" : : "r"(v));
    gUnknown_030020A8.unk04 = NULL;
    gUnknown_030020A8.unk00 = 0;
    (*pp)->unk04 = NULL;
}
asm(".global sub_0801A6C0\n.thumb_set sub_0801A6C0, ResetSpriteRequestQueue\n");
