#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080520B8.
 * sub_080520B8 @ 0x080520B8
 */

/* MATCHED. The guarded half of the DeathHandler_Tank / DeathHandler_Air install
 * sequence: publish the side and slot, then allocate the sprite ONLY if the
 * slot does not already hold one (`unk24[b] == -1`). It is the function that
 * names gUnknown_085536D4, the animation descriptor whose draw slot is the
 * matched AirBlastEffect_Init -- see the descriptor table in
 * include/unknown-globals.h.
 *
 * `f` is `u16` here and `int` in DeathHandler_Tank / DeathHandler_Air, and that is
 * measured, not a style choice: the ROM narrows the `* 7 + 6` product at the
 * ASSIGNMENT (`lsls #0x10; lsrs #0x10` into r7) and again to u8 at the call
 * (`lsls #0x18; lsrs #0x18`). The exemplars have only the second pair, which is
 * what an `int` local gives. Two shift pairs = a narrow local, one = `int`. */
void SpawnDebrisEffect(u16 a, u16 b)
{
    u16 f;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    f = gUnknown_08552178[a][b] * 7 + 6;

    if (gUnknown_02029808[a].unk24[b] == -1)
    {
        gUnknown_02029808[a].unk24[b] = sub_08015410(gUnknown_085536D4, 1,
            gUnknown_02029808[a].unk44[gUnknown_02029808[a].unk2e],
            gUnknown_02029808[a].unk58[gUnknown_02029808[a].unk2e],
            f);
        PlayFigureDestroySfx(a, 0x10);
    }
}
asm(".global sub_080520B8\n.thumb_set sub_080520B8, SpawnDebrisEffect\n");
