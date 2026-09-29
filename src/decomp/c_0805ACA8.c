#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805ACA8.
 * sub_0805ACA8 @ 0x0805ACA8, sub_0805ACFC @ 0x0805ACFC
 */

/* The 0x0805ACA8 driver's per-cell tester, and the near-twin of AiCheckDropNeighbour
 * in src/decomp/c_08058BB4.c: same guards, same `rowOffset[y] + x` key, same
 * (s8) read of gUnknown_03003340. It differs only in the tail -- 0x78 is
 * rejected as well as 0x79, and the terrain gate accepts exactly the two
 * codes 0xb and 0xd instead of consulting the move table.
 *
 * sub_0805C128 is the same shape again with a different tail.
 *
 * struct Map is the wave-35 canonical layout, copied verbatim from
 * c_08058BB4.c -- gUnknown_08499590 stays `u8 *` and the byte arithmetic is
 * written through a locally declared struct cast onto it, which is what keeps
 * `(map + K) + idx` from folding K into the load displacement.
 *
 * The single `lsls #0x18; asrs #0x18` serves all three compares, so it is an
 * int local carrying one explicit (s8) cast, not an s8 local -- the same
 * reading c_08058BB4.c records for AiCheckDropNeighbour. */

/* Offers the four cells orthogonally adjacent to (x, y) to AiCheckShoalParkNeighbour and
 * reports whether any of them was accepted. The exact shape of AiPickFiringCellBeside
 * in src/decomp/c_08058BB4.c, with a 1/0 result instead of best/-1.
 *
 * BINDING THE SENTINEL IS LOAD-BEARING, for the reason c_08058BB4.c records:
 * the literal 0x270F written at both the seed store and the final test stays a
 * plain constant, loaded into r0 and stored from r0, so the ROM's redundant
 * `ldr r0,=0x270F; mov r8,r0; mov r0,r8; strh` has nothing to come from. Its
 * own statement makes it a pseudo whose live range starts before the store, so
 * the store must read it back out of the callee-saved register.
 *
 * The tail is written `== sentinel -> 0` and not `!= sentinel -> 1`: agbcc
 * branches to the `if` BODY and falls through to what follows, so the arm
 * written as the body is the one laid out after the literal pool. The ROM's
 * `beq` reaches `movs r0,#0`, so `return 0` is the body. The opposite spelling
 * compiles to the same instructions in the opposite order.
 *
 * The fourth call's `adds r4,#1` clobbers y because y is dead after it; that
 * falls out of writing `y + 1` last. */
u8 AiPickShoalParkBeside(int x, int y, u16 * out)
{
    int sentinel;

    sentinel = 0x270F;
    out[0] = sentinel;

    AiCheckShoalParkNeighbour(x - 1, y, out);
    AiCheckShoalParkNeighbour(x + 1, y, out);
    AiCheckShoalParkNeighbour(x, y - 1, out);
    AiCheckShoalParkNeighbour(x, y + 1, out);

    if (out[0] == sentinel)
        return 0;

    return 1;
}
asm(".global sub_0805ACA8\n.thumb_set sub_0805ACA8, AiPickShoalParkBeside\n");

void AiCheckShoalParkNeighbour(int x, int y, u16 * out)
{
    struct Map * map;
    int idx;
    int t;
    int terrain;

    if (x < 0)
        return;
    if (y < 0)
        return;

    map = gMap;

    if (x >= map->width)
        return;
    if (y >= map->height)
        return;

    idx = map->rowOffset[y] + x;

    if (gMap->unit[idx] != gUnknown_03003F38 && gMap->unit[idx] != 0)
        return;

    t = (s8)gUnknown_03003340[y][x];

    if (t < 0)
        return;
    if (t == 0x78)
        return;
    if (t == 0x79)
        return;

    terrain = map->terrain[idx] & 0x1f;

    if (terrain != 0xd && terrain != 0xb)
        return;

    out[0] = x;
    out[1] = y;
}
asm(".global sub_0805ACFC\n.thumb_set sub_0805ACFC, AiCheckShoalParkNeighbour\n");
