#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080523E8.
 * sub_080523E8 @ 0x080523E8
 */

/* MATCHED -- wave 80 (W80-B), byte-exact, relocs match. Same lever as
 * DeathHandler_Explosion / DeathHandler_Bomb: a dead `k = b * 2;` after
 * `gUnknown_0300451C = b;` with the plain subscript kept everywhere. This is
 * what the wave-73 note's "comma at a different depth" was reaching for,
 * except it is not a comma at all: the reference is created by a statement
 * that generates the pseudo and nothing else, and cse connects it to the
 * store's index after the call, where get_inner_reference has put b * 2
 * first in the offset sum so the sum ties to it. The third parameter is
 * byte-neutral (wave 51) and kept for the RunDeathBatch table evidence. */
void DeathHandler_Splash(u16 a, u16 b, int c)
{
    u16 t;
    int k;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    k = b * 2;

    gUnknown_02029808[a].unk24[b] =
        sub_08015410(gUnknown_085536EC, 1,
                     gUnknown_02029808[a].unk44[gUnknown_02029808[a].unk2e],
                     gUnknown_02029808[a].unk58[gUnknown_02029808[a].unk2e], 0);

    SetSlotSpriteFlicker(gUnknown_02029808[a].unk24[b], 1);

    m4aSongNumStop(0x3B);
    m4aSongNumStop(0x3C);
    PlayMusicOrSfx(0x51);

    t = gUnknown_08553B14[a];

    EnableSlotSpriteAffine(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteDoubleSize(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], t);
    SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x100);

    DeathHandler_CommonTail(a, b);
}
asm(".global sub_080523E8\n.thumb_set sub_080523E8, DeathHandler_Splash\n");
