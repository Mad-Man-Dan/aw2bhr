#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026C6C.
 * sub_08026C6C @ 0x08026C6C
 */

/* A membership test over unit/terrain ids 6..20 that returns the shared
 * gPlaySt.propertyFunds word for six of them and 0 otherwise. agbcc emits a
 * jump table because the fifteen labels are dense, and the six hits all share
 * one body, so the table holds the same target six times.
 *
 * A leaf: no `push`, and the epilogue is a bare `bx lr`. */
u32 GetIncomeForTerrainKind(u8 a)
{
    switch (a)
    {
    case 6:
    case 8:
    case 10:
    case 11:
    case 14:
    case 20:
        return gPlaySt.propertyFunds;
    }
    return 0;
}
asm(".global sub_08026C6C\n.thumb_set sub_08026C6C, GetIncomeForTerrainKind\n");
