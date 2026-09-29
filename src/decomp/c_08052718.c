#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052718.
 * sub_08052718 @ 0x08052718
 */

/* MATCHED -- wave 80 (W80-B). Byte-exact; the only reloc difference is the
 * -fforce-addr .rodata word for gUnknown_02029808 (the ROM symbolises it as
 * gUnknown_0813610C, a .LC slot in data/data.s), which promotion carries as
 * "rodata": ["0x0813610C"]. Honest spelling, no c_local.
 *
 * THE LEVER, after twelve waves parked on "the ROM computes b * 2 before the
 * sub_08015410 argument setup and holds it across the call": a DEAD
 * STATEMENT. `k = b * 2;` written right after `gUnknown_0300451C = b;` and
 * never read. cse runs before flow deletes dead insns, finds the identical
 * `(ashift b 1)` in the store's index after the call and substitutes k's
 * pseudo -- so the shift is emitted where the statement is and the pseudo is
 * live across the call. The store keeps the PLAIN subscript
 * gUnknown_02029808[a].unk24[b], so get_inner_reference's offset tree is
 * (b * 2 + a * 108) with b * 2 FIRST and local_alloc ties the post-call sum
 * to k's register (r5), not to a * 108's. W77-K's
 * `*(s16 *)((int)gUnknown_02029808[a].unk24 + k)` form reaches size-exact
 * 94.9% but expand's both_summands canonicalisation puts a * 108 first and the
 * sum ties the other way (r4/r5/r6 permuted); writing `k + (int)...` is
 * byte-identical to it, and using it for all six reads is worse (it sinks
 * +0x24 into a tail pool word). Everything else is as earlier waves recorded:
 * `a ^ 1`, `e` a local, 0x100 as movs/lsls, the control-flow merge being why
 * this twin has the .rodata word and DeathHandler_Bomb does not. */
void DeathHandler_Explosion(u16 a, u16 b)
{
    u16 e;
    int k;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    k = b * 2;

    gUnknown_02029808[a].unk24[b] = sub_08015410(gUnknown_08553704, 1,
        gUnknown_02029808[a].unk44[gUnknown_02029808[a].unk2e],
        gUnknown_02029808[a].unk58[gUnknown_02029808[a].unk2e],
        0);

    SetSlotSpriteFlicker(gUnknown_02029808[a].unk24[b], 1);

    if (gUnknown_03004580[a ^ 1][1] == 0x14)
        PlayFigureDestroySfx(a, 0x23);
    else
        PlayFigureDestroySfx(a, 0x10);

    e = gUnknown_08553B14[a];

    EnableSlotSpriteAffine(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteDoubleSize(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
    SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x100);

    DeathHandler_CommonTail(a, b);
}
asm(".global sub_08052718\n.thumb_set sub_08052718, DeathHandler_Explosion\n");
