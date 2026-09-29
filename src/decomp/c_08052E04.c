#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052E04.
 * DeathHandler_Fall @ 0x08052E04
 */

/* MATCHED, first draft. StartFigureFall's wrapper, and a callee of the wave-20
 * SplashEffect_Loop / BattleAnimExplosion_Loop pair and of the matched BombEffect_Init: publish the
 * side and slot, then tear the slot down if it is not already flagged.
 *
 * The third parameter is DEAD in the body and every caller still passes an
 * explicit 0. Dropping it from the prototype changes nothing the callee can see
 * and breaks all three call sites, so it stays.
 *
 * Same inline-expression rule as StartFigureFall: writing
 * `gUnknown_02029A10[a].entries[b]` out at each use reproduces the ROM's
 * pool-word-then-`b * 0x24`-then-`a * 0xb4` order, and binding it to a pointer
 * local inverts the two offset terms for +1 instruction.
 *
 * `entries[b].unk18 = -1` comes out as `ldr r0, =0x0000FFFF; strh` rather than
 * a `movs`/`negs` pair, which is agbcc's own choice for the halfword store and
 * needs nothing in the source. */
void DeathHandler_Fall(u16 a, u16 b, int c)
{
    gUnknown_0300453C = a;
    gUnknown_0300451C = b;

    if (gUnknown_02029A10[a].entries[b].unk01 == 0
        && gUnknown_02029A10[a].entries[b].unk18 != -1)
    {
        StartFigureFall(a, b);
        gUnknown_02029B80[a][b] = 1;
        sub_080156E8(gUnknown_02029A10[a].entries[b].unk18,
            gUnknown_02029BA8[a].unk14);
        gUnknown_02029A10[a].entries[b].unk18 = -1;
    }
}

asm(".global sub_08052E04\n.thumb_set sub_08052E04, DeathHandler_Fall\n");
