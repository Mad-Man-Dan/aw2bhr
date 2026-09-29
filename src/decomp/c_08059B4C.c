#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059B4C.
 * sub_08059B4C @ 0x08059B4C
 */

/* Wave 52, W52-B.  MATCHED (hand to 95.0%, permuter closed the rest).
 *
 * TWO SPELLINGS THAT LOOK IDENTICAL AND ARE NOT, both measured here and both
 * written up in docs/agbcc-codegen.md:
 *  - `(&arr[w])->unk03[k]` matches; `arr[w].unk03[k]` costs 4 bytes (an r0/r1
 *    swap around the `ldrsb`).  Do NOT tidy this into the member form.
 *  - `arr` must be bound in its OWN statement AFTER `w` is read.  Written as
 *    `&gUnknown_084995A0[w]` in place, the pool `ldr` is scheduled between the
 *    pointer global's two loads instead of before them.
 *
 * gUnknown_03004788 is VOLATILE and this function is what proves it: the ROM
 * keeps its ADDRESS in r2 and issues a SECOND `ldr r1,[r2]` for the clamp,
 * where CSE otherwise folds the assignment to `adds r1,r0,#0`.  See that
 * symbol's comment in include/unknown-globals.h; AiCountEnemyFacilities, the only other
 * reader, was re-verified byte-exact after the change.
 *
 * The loop is a `goto` loop, as its sibling c_0805B778.c is: nothing is
 * LICM-hoisted into a preheader -- the 0x270F, the sp alias and
 * gMap are all re-formed on every iteration.
 *
 * The cell lookup is gMap->property[gMap->rowOffset[y] + x], read as s8.
 */

void AiAllocateTerritoryTarget(int a1, int a2, int a3, void *a4, void *a5)
{
    union Unk802C57CBuf v;
    struct PropertyListEntry *arr;
    u8 k;
    int w;
    int n;

    k = a3;

    if (a2 > gUnknown_03004788)
        a2 = gUnknown_03004788;

    n = a1 / a2;

loop:
    v.pos.unk00 = 0x270F;
    AiPopLastNearestCandidate(a4, (u16 *)&v);

    if (v.pos.unk00 == 0x270F)
        return;

    w = gMap->property[gMap->rowOffset[v.pos.unk02] + v.pos.unk00];
    arr = gUnknown_084995A0;

    if (n < (&arr[w])->unk03[k])
        goto loop;

    (&arr[w])->unk03[k]++;

    ((u16 *)a5)[0] = v.pos.unk00;
    ((u16 *)a5)[1] = v.pos.unk02;
}
asm(".global sub_08059B4C\n.thumb_set sub_08059B4C, AiAllocateTerritoryTarget\n");
