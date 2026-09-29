#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086058.
 * sub_08086058 @ 0x08086058
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Family F062 (data/families.json): `push {r4,lr}; adds r4,r0,#0; bl A;
 * adds r0,r4,#0; bl B; pop {r4}; pop {r0}; bx r0` -- 20 bytes, three members.
 * Both `adds` immediates are absent from `varies`, so only the two callees
 * differ.
 *
 * This is NOT the wave-14 F032 shape: there is no `lsls r0,#0x18; cmp r0,#0;
 * bne`, so the second call is unconditional and the first callee's result is
 * discarded. Whether the FIRST call receives the incoming r0 is decided
 * callee-side and the three members do not agree on it -- see each below.
 * `pop {r4}; pop {r0}` is still the void epilogue.
 */


/* The OPPOSITE reading to sub_0801153C, in the same family, which is why every
 * member has to be checked rather than copied: MapSelectList_StepScrollRedraw really does take
 * the proc -- it opens `adds r4,r0,#0` and dereferences +0x30, +0x4e and +0x5c
 * -- so both calls receive it. sub_0808603C next door drives the same object
 * through MapSelectList_StepScrollRedraw, MapSelectList_HandleInput and MapSelectList_DrawFrame in a row.
 *
 * Wave 44 (W44-C) retyped MapSelectList_StepScrollRedraw's parameter to its own struct; ProcPtr
 * is `void *`, so the argument converts implicitly here and this stays
 * byte-for-byte identical. Re-verified with the new declaration in place.
 */

void MapSelectList_DrawLoop(ProcPtr proc)
{
    MapSelectList_StepScrollRedraw(proc);
    MapSelectList_DrawFrame(proc);
}

asm(".global sub_08086058\n.thumb_set sub_08086058, MapSelectList_DrawLoop\n");
