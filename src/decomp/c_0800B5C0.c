#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800B5C0.
 * sub_0800B5C0 @ 0x0800B5C0
 */

/* Family F061: "OR together the four orthogonal neighbours of (x, y)", one
 * body three times over three different cell predicates -- data/families.json
 * `varies` is four `bl S` entries and nothing else, and within each member all
 * four callees are the same function.
 *
 * gUnknown_08499590 is named twice, once per bound test, and -fforce-addr
 * collapses the two references to a single pool `ldr` while leaving both
 * `ldr r0,[r7]` derefs in place (the intervening `bl` kills the cached load).
 * Binding a pointer local instead loses the second deref.
 *
 * The first neighbour is a plain assignment and not `|=`: r6 is provably 0 on
 * that path, so `orrs` folds away either way, but the assignment is what the
 * source reads as. The first call sets up only r1 because the incoming r0
 * still holds x.
 *
 * `int` and not bool8 on the callee return: every result goes straight into
 * `lsls #3`/`#2`/`#1` or a bare `orrs`, with no `lsls #0x18; lsrs #0x18` in
 * front of it, and agbcc re-narrows a narrow-returning callee at every call
 * site. */

int GetShoalNeighbourMask(int x, int y)
{
    int r = 0;

    if (y > 0)
        r = IsShoalAt(x, y - 1) << 3;

    if (x > 0)
        r |= IsShoalAt(x - 1, y) << 2;

    if (x < gMap->width - 1)
        r |= IsShoalAt(x + 1, y) << 1;

    if (y < gMap->height - 1)
        r |= IsShoalAt(x, y + 1);

    return r;
}
asm(".global sub_0800B5C0\n.thumb_set sub_0800B5C0, GetShoalNeighbourMask\n");
