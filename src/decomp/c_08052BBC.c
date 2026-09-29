#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052BBC.
 * sub_08052BBC @ 0x08052BBC
 */

/* MATCHED -- wave 80 (W80-B), byte-exact, relocs match. Two DEAD statements
 * do both hoists that W77-K reproduced with cast-and-offset arithmetic:
 * `k = b * 2; j = a * 4;` right after the gUnknown_0300453C / 0300451C
 * stores, never read, and every access keeps its plain subscript --
 * gUnknown_02029808[a].unk24[b] everywhere, `gUnknown_02028E5C[a][0] = 1` as
 * the LAST statement where its pool word belongs (slot 6). cse substitutes
 * the two pseudos at the later identical index expressions, the plain
 * subscripts keep get_inner_reference's operand order (inner index first) so
 * the post-call sum ties to k, and the register split the wave-77 entry
 * called "why does the longer-ranged allocno win the lo register" was never
 * an allocation question: it was which operand of the sum local_alloc ties.
 * Wave-77 measurements that still stand: declaration order of j/k is
 * byte-neutral; gUnknown_0855371C is an animation descriptor; DeathHandler_CommonTail
 * narrows both u16 parameters in place; `e` is a local. See
 * docs/agbcc-codegen.md, "A DEAD `k = i * S;` statement is a hoist lever". */
void DeathHandler_Bomb(u16 a, u16 b)
{
    u16 e;
    int k;
    int j;

    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    k = b * 2;
    j = a * 4;

    gUnknown_02029808[a].unk24[b] = sub_08015410(gUnknown_0855371C, 1,
        gUnknown_02029808[a].unk44[gUnknown_02029808[a].unk2e],
        gUnknown_02029808[a].unk58[gUnknown_02029808[a].unk2e],
        0);

    SetSlotSpriteFlicker(gUnknown_02029808[a].unk24[b], 1);
    PlayFigureDestroySfx(a, 0x23);

    e = gUnknown_08553B10[a];

    EnableSlotSpriteAffine(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteDoubleSize(gUnknown_02029808[a].unk24[b]);
    SetSlotSpriteScaleX(gUnknown_02029808[a].unk24[b], e);
    SetSlotSpriteScaleY(gUnknown_02029808[a].unk24[b], 0x180);

    gUnknown_02028E5C[a][0] = 1;

    DeathHandler_CommonTail(a, b);
}
asm(".global sub_08052BBC\n.thumb_set sub_08052BBC, DeathHandler_Bomb\n");
