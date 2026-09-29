#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029948.
 * StartDropCellPicker @ 0x08029948
 */

/* MATCHED -- wave 60 (W60-D, from W60-A's and the orchestrator's exemplars).
 * 48/48 bytes, relocs match.
 *
 * THE LEVER IS `s16 id`, AND THE PARK MISSED IT BY ENUMERATING THE WRONG
 * FAMILY. The ROM interleaves the array's force-addr load INTO the
 * sign-extension of sub_080152C0's s8 return:
 *     lsls r0, r0, #0x18
 *     ldr  r2, =gUnknown_03001470
 *     asrs r0, r0, #0x18
 * The index local's DECLARED TYPE is what decides where that `ldr` lands:
 *     s8  id  ->  ldr ahead of BOTH shifts
 *     int id  ->  ldr behind BOTH shifts
 *     s16 id  ->  ldr BETWEEN them, which is the ROM
 * Wave 31 measured the first two and a `struct Unk03001470 *` binding, and
 * concluded no source spelling could reach the middle. It never tried s16.
 *
 * NOTE THE TRAP THAT HIDES IT: the extend is `#0x18`, i.e. EIGHT bits, so
 * reading the local's width off the extend width picks `s8` -- which is the
 * one spelling guaranteed to put the load in the wrong place. The extend
 * width belongs to the CALLEE's return type; the local's width is a separate
 * fact and here they differ. `s16 id` needs no extra narrowing because
 * num_sign_bit_copies already proves an s8-derived value fits s16, so the
 * choice is free in instruction count and visible only in the load's slot.
 *
 * This is the same recipe as the three already-promoted functions that
 * contain the identical interleave against the identical global -- read
 * src/decomp/c_08035BC4.c, whose own comment states the s8/int/s16 result
 * outright, and src/decomp/c_080149C0.c (PutTextScriptImmediate and PutTextTableEntryImmediate).
 * W60-A's scan of asm/ found 92 interleaved extend pairs across 67 distinct
 * functions with the large majority already matched, so this placement is
 * ordinary compiler output, not an unreachable artefact.
 *
 * THE OTHER HALF OF THE FIX, and it had nothing to do with the interleave:
 * the draft had silently drifted 4 bytes SHORT (44 bytes, 27.1%) at some
 * point before this wave. `... .unk22 = (u16)a;` writes the cast straight
 * into a `strh`, which truncates anyway, so combine deletes the narrowing
 * entirely; the ROM materialises it before the call. Binding the value to a
 * `u16` LOCAL restores it, because a promoted HImode pseudo must actually
 * hold a zero-extended value and combine may not fold that away. Nobody
 * edited the draft -- sub_080152C0's return type was corrected to `s8` in
 * include/unknown-functions.h for another function's sake, which is what let
 * the cast fold. An explicit `(s8)` on the call result changes nothing, since
 * the prototype already says s8.
 */
void StartDropCellPicker(int a)
{
    u16 v;
    s16 id;

    v = a;
    id = sub_080152C0((s32)gUnknown_0849A080, 0);
    gUnknown_03001470[id].unk22 = v;
}
asm(".global sub_08029948\n.thumb_set sub_08029948, StartDropCellPicker\n");
