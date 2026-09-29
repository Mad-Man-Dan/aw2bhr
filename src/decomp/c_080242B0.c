#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080242B0.
 * sub_080242B0 @ 0x080242B0
 */

/* Wave 80 (W80-A): MATCHED by writing the `return TRUE` body FIRST in the
 * switch. The wave-35/39/77 residual was the case-14 leaf's branch polarity
 * (`beq true; b default` vs the ROM's `bne default; b true`). gcc 2.95's
 * expand_end_case emits the decision tree and reorder_insns it to the START of
 * the switch, so the block that follows the tree's last test (the right leaf,
 * {14}) is the FIRST case body in SOURCE order. When that body is the shared
 * `return TRUE`, jump.c inverts the leaf's `beq L; b default; L:` into
 * `bne default; L:` because L is the next block, and cross-jumping then folds
 * the `movs r0,#1; b end` into case 6's identical tail, leaving `bne default;
 * b <tail>` -- the ROM. Case-LABEL order within a group is irrelevant (W39-C
 * measured it); case-BODY order is the lever. See the wave-80 chapter in
 * docs/agbcc-codegen.md. */

u8 CanBuildAtCell(s16 a1, s16 a2)
{
    struct Map *map;
    int idx;
    int cell;
    int hi;
    int lo;

    map = gMap;
    idx = map->rowOffset[a2] + a1;
    cell = map->terrain[idx];
    hi = cell & 0xe0;
    lo = cell & 0x1f;

    if (gMap->unit[idx] != 0)
        return FALSE;

    if (hi != gUnknown_03004084)
        return FALSE;

    switch (lo)
    {
    case 10:
    case 11:
    case 14:
        return TRUE;

    case 6:
        if (GetPlayerSpecialAbilities(gUnknown_030033EC) & 2)
            return TRUE;
        break;
    }

    return FALSE;
}
asm(".global sub_080242B0\n.thumb_set sub_080242B0, CanBuildAtCell\n");
