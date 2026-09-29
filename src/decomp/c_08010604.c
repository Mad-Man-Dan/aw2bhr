#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010604.
 * GetSeamType @ 0x08010604
 */

/* The map header IsRoadOrBridgeAt and its neighbours walk, seen through the LOCAL
 * struct spelling recorded on gUnknown_08499590 in include/unknown-globals.h.
 * unk0A22 is a REFINEMENT of the canonical `u8 unitUnk[0x0F18]` used by the
 * twelve promoted files that already declare `struct Map`: that run splits at
 * 0x0A22 into a u16 plane of the same 0x508 cells as unk0012, indexed by the
 * same `unk417A[y] + x` key, and 0x0A22 + 0x508 * 2 lands exactly on unk1432.
 * Every access here is `(map + K) + idx`, which only the COMPONENT_REF spelling
 * preserves. */

/* Maps a road/bridge cell id onto the id its neighbours should be drawn with:
 * the 0x142/0x162 pair collapses to 0x162, the 0x143/0x163 pair to 0x163, and
 * anything else to 0x180. The `add r0,#0x20` / `add r0,#1` chains are agbcc
 * reusing the previous compare's constant, not a source-level expression. */
int GetSeamType(int x, int y)
{
    u16 t;

    t = gMap->tile[
            gMap->rowOffset[y] + x];
    if (t == 0x142 || t == 0x162)
        return 0x162;
    if (t == 0x143 || t == 0x163)
        return 0x163;
    return 0x180;
}

asm(".global sub_08010604\n.thumb_set sub_08010604, GetSeamType\n");
