#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080079B8.
 * sub_080079B8 @ 0x080079B8
 */

/*
 * sub_080079B8 -- write a1 into the design ring slot the cursor points at.
 *
 * The slot is gActiveMap->ringIndex moved on by 4 in terrain mode or 3 in unit
 * mode, wrapping at 10. That entry's itemId becomes a1.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `m` and the `while (m == 0) m = 1;` loop keep a computation the original
 *     performs and never uses: it adds 4 to terrainListIndex, or 3 to
 *     unitListIndex, and throws the result away. The compiler drops dead code
 *     only once, early on, so the value has to still have a user at that point
 *     and lose it later; this one-trip loop is the cheapest way to arrange
 *     that. An unused local, an `if` in place of the `while`, or `volatile`
 *     all lose the instructions. The loop must compare against 0 and the
 *     addends must stay literal numbers. What the original source wrote here
 *     cannot be recovered from the bytes; this is only a way to reproduce them.
 *   - `c = p->editMode;` must stay ahead of `i = n + 3;`. The original loads
 *     editMode first, and swapping the lines swaps the instructions.
 */

void sub_080079B8(int a1)
{
    struct ActiveMap *p;
    int n;
    int c;
    int i;
    int m;

    p = gActiveMap;
    n = p->ringIndex;
    c = p->editMode;
    i = n + 3;
    if (c == 0)
        i = n + 4;
    if (i > 9)
        i -= 10;
    if (c == 0)
        m = (s8)p->terrainListIndex + 4;
    else
        m = (s8)p->unitListIndex + 3;
    while (m == 0)
        m = 1;
    gDesignRing[i].itemId = a1;
}
