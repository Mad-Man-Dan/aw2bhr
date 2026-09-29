#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08008CB8.
 * sub_08008CB8 @ 0x08008CB8, sub_08008D14 @ 0x08008D14
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

int sub_08008CB8(int x, int y)
{
    int r = 0;

    if (y > 0)
        r = sub_0800977C(x, y - 1) << 3;

    if (x > 0)
        r |= sub_0800977C(x - 1, y) << 2;

    if (x < gMap->width - 1)
        r |= sub_0800977C(x + 1, y) << 1;

    if (y < gMap->height - 1)
        r |= sub_0800977C(x, y + 1);

    return r;
}

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

int GetLandNeighbourMask(int x, int y)
{
    int r = 0;

    if (y > 0)
        r = IsTerrainLand(x, y - 1) << 3;

    if (x > 0)
        r |= IsTerrainLand(x - 1, y) << 2;

    if (x < gMap->width - 1)
        r |= IsTerrainLand(x + 1, y) << 1;

    if (y < gMap->height - 1)
        r |= IsTerrainLand(x, y + 1);

    return r;
}
asm(".global sub_08008D14\n.thumb_set sub_08008D14, GetLandNeighbourMask\n");
