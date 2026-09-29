#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087B60.
 * sub_08087B60 @ 0x08087B60
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


/* Third reading again: here the saved value is an INT, not a proc. Both callees
 * add it to a global's address as an offset (&gUnknown_02027F74 + 4 in
 * LoadEnemyCoMinimugs, &gUnknown_02027F78 in LoadMapRecordMinimugs) and both stash it at +0x54
 * of the proc they find by script; LoadEnemyCoMinimugs was already declared `int` on
 * that evidence.
 */

void RefreshMapSelectSidePanels(int a)
{
    LoadEnemyCoMinimugs(a);
    LoadMapRecordMinimugs(a);
}
asm(".global sub_08087B60\n.thumb_set sub_08087B60, RefreshMapSelectSidePanels\n");
