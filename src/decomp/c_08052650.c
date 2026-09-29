#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052650.
 * sub_08052650 @ 0x08052650
 */

/* MATCHED. The end-of-animation half of the tick: bump the counter and, on the
 * last frame only, hand the slot back -- optionally through DeathHandler_Fall --
 * and clear the two per-side scratch words the install sequence set.
 *
 * BattleAnimExplosion_Loop IS THIS FUNCTION, byte for byte, and not by coincidence: they
 * are slot 2 of gUnknown_085536EC and gUnknown_08553704, two different
 * animation descriptors, so one C body needs two addresses. Neither has a `bl`
 * caller. See the descriptor table in include/unknown-globals.h.
 *
 * The wave-20 brief predicted this needs the `c = a; d = b;` copy pair
 * src/decomp/c_08051F4C.c documents, because the ROM loads unk30/unk34 into
 * r4/r3 and copies each into r5/r6. IT DOES NOT, and the copies are actively
 * wrong: with them, cse propagates `a` into every use of `c` in this single
 * basic block, the copy dies, the live ranges merge, and the output comes out
 * one saved register SHORT of the ROM. The ROM's `adds rD, rS, #0` pairs are
 * reload artefacts -- one value live across the DeathHandler_Fall call needing a
 * callee-saved home -- and the two registers are never live at the same
 * instruction, which is the test. c_08051F4C's copies survive only because
 * their last use sits inside a nested `if`.
 *
 * `unk1c` is new, and it is typed by an independent pair rather than by this
 * read alone: DeathHandler_CommonTail writes the same field `movs r0,#1; strh r0,[r4,#0x1c]`.
 */
void SplashEffect_Loop(void)
{
    u16 a;
    u16 b;
    u16 e;

    gUnknown_03001470[gUnknown_03001FBC].unk28++;

    if (gUnknown_03001470[gUnknown_03001FBC].unk28 == 8)
    {
        a = gUnknown_03001470[gUnknown_03001FBC].unk30;
        b = gUnknown_03001470[gUnknown_03001FBC].unk34;

        e = gUnknown_08553B10[a];

        if (gUnknown_02029A10[a].entries[b].unk1c == 1)
            DeathHandler_Fall(a, b, 0);

        gUnknown_02028E5C[a][0] = 1;
        *gUnknown_084C3F78[a] = 0;

        SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
        SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x180);
    }
}
asm(".global sub_08052650\n.thumb_set sub_08052650, SplashEffect_Loop\n");
