#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080733C8.
 * sub_080733C8 @ 0x080733C8
 */

/* MATCHED, wave 46 (W46-A), fourth attempt.
 *
 * Counts, for every character of the string at a2, how many map cells carry a
 * terrain id equal to it: zero the counters, then walk the gUnknown_03003F68
 * render buffer (u8 width at +0, u8 height at +1, u16 tiles from +2) mapping
 * each tile through gUnknown_0849959C and masking to 5 bits.
 *
 * TWO READOUTS, EACH WORTH AN ATTEMPT.
 *
 * 1. `int i = 0;` must be a DECLARATION INITIALISER, not the `for` init. The
 *    ROM sets the counter to zero BEFORE `bl LoadMapIntoGMap`, which puts its live
 *    range across the call and earns it a callee-saved register; written as
 *    `for (i = 0; ...)` the store lands after the call and the counter takes a
 *    scratch. 88.0% -> 98.9%.
 *
 * 2. THE BUFFER POINTER NEEDS ITS OWN TWO STATEMENTS (wave-43 W43-I rule, and
 *    its second half). Spelled inline the address `adds` puts the sum in the
 *    INDEX's register; the ROM puts it in the base's. That was the entire
 *    2-byte residual of a 98.9% candidate, and no operand reordering inside the
 *    expression moved it. The `+ 1` stays in the load displacement either way,
 *    so this is the binding and not the struct-vs-pointer question.
 *
 * The buffer must stay a cast rather than a struct for the same reason
 * src/decomp/c_080377C4.c records: the ROM leaves the +2 in the `ldrh`
 * displacement, which a COMPONENT_REF would spend an instruction on. The global
 * is re-read at both loop bounds rather than bound to a local -- ordinary
 * output for a `void *` global that a store might alias. */

void CountMapTilesOfTerrainKinds(int a1, const void * a2, void * a3)
{
    const u8 *str = a2;
    u8 *counts = a3;
    int i = 0;
    int j;
    int x;
    int y;
    int t;
    u16 *tile;

    LoadMapIntoGMap(a1);

    for (; str[i] != 0; i++)
        counts[i] = 0;

    for (y = 0; y < ((u8 *)gUnknown_03003F68)[1]; y++)
    {
        for (x = 0; x < ((u8 *)gUnknown_03003F68)[0]; x++)
        {
            tile = (u16 *)gUnknown_03003F68;
            tile += y * ((u8 *)gUnknown_03003F68)[0] + x;

            t = gUnknown_0849959C[tile[1]] & 0x1f;

            for (j = 0; str[j] != 0; j++)
            {
                if (t == str[j])
                    counts[j]++;
            }
        }
    }
}
asm(".global sub_080733C8\n.thumb_set sub_080733C8, CountMapTilesOfTerrainKinds\n");
