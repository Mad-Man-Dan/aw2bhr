#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F888.
 * CacheUnitMovementCosts @ 0x0801F888, SetWorkingMapPlane @ 0x0801F92C, sub_0801F98C @ 0x0801F98C
 */

/* Loads the terrain movement-cost row for unit type a1 into the flood fill's
 * 0x20-byte cost table at gUnknown_084999C8->unk00 -- the table MapFloodCoreStep
 * then charges each step against. The source row is
 * gUnknown_085D3DD0[..].unk38[..].unk18[..], indexed by the unit's movement
 * type gUnknown_085D5ABC[a1].unk19 * 32, with the terrain code as the column,
 * so this is the per-type slice of the same table c_08041EA8.c reads per-cell.
 *
 * The whole lookup is re-done on every one of the 32 iterations because the
 * `strb` into gUnknown_084999C8's pointee kills every load -- the same reason
 * c_0801F838.c re-reads its bounds. Nothing here is hoisted by hand.
 *
 * The table selection is c_08038848.c's expression with a different army
 * index (gUnknown_03004480, no `+ 1`): the `?:` picks row unk1d or the literal
 * 1, which is why the else arm is the bare 0x104 == 1 * sizeof(struct
 * CoData), and `(unk1e * 17 + unk2c) << 2` is unk38's 0x44 stride sharing
 * an `lsls #2` with unk18's 4-byte element stride off the +0x50 member base.
 *
 * THE THREE LOCALS ARE LOAD-BEARING, in this order, and each was measured
 * against its inlined form this wave:
 *   - `dst` first: the ROM computes gUnknown_084999C8 + i before touching
 *     anything else. Without it the store address is still computed first, but
 *     see `costs`.
 *   - `costs`: binding the row pointer is what puts `ldr r1,[r0]` immediately
 *     after `add r0, sl`. Inlined, agbcc defers the load past the
 *     gUnknown_085D5ABC lookup and needs an extra `mov rN, sl` to hold the base
 *     (+2 bytes), because the ternary's result no longer dies into the load.
 *   - `c`: the cell index must be its own statement to keep `adds r0, r4, r0`,
 *     i + row. Written inline as `costs[i + ...]` the sum's target IS the
 *     register already holding `unk19 * 32`, so expand_binop takes the
 *     target-reuse swap and emits `adds r0, r0, r4` -- the operands come out
 *     reversed no matter which order the source writes them in. Same lever
 *     c_08038848.c records for the same subexpression. */
void CacheUnitMovementCosts(int a1)
{
    int i;
    u8 *dst;
    s8 *costs;
    int c;

    for (i = 0; i < 32; i++) {
        dst = &gUnknown_084999C8->unk00[i];
        costs = gUnknown_085D3DD0[gPlaySt.coAbilities
                    ? gPlayers[gUnknown_03004480].co
                    : 1]
                .power[gPlayers[gUnknown_03004480].coMode]
                .movementChart[gPlaySt.weather];
        c = i + gUnknown_085D5ABC[a1].movementType * 32;
        *dst = costs[c];
    }
}

asm(".global sub_0801F888\n.thumb_set sub_0801F888, CacheUnitMovementCosts\n");

/* Rebuilds the gUnknown_03003340 row-pointer table FillMovementMap and friends
 * write through: row y of the caller's plane starts at `a1 + rowOffset[y]`,
 * where rowOffset is the +0x417A halfword table of the gUnknown_08499590 map.
 * Then it publishes the map's width and height as the u8 pair at
 * gUnknown_084999C8 +0x28 / +0x29 (the bounds MapFloodCoreStep's flood fill reads).
 *
 * `stm r7!, {r0}` is NOT authored as a walking pointer. Plain
 * `gUnknown_03003340[y] = ...` is what produces it -- strength_reduce turns the
 * word-scaled subscript into a post-increment giv, and the giv's init `ldr
 * r7,=gUnknown_03003340` lands in the preheader AFTER the zero-trip guard,
 * which is where the ROM has it. Binding `p = gUnknown_03003340` before the
 * loop is a source-level pseudo instead, so its `ldr` is emitted with the
 * binding statement ahead of the guard, it reorders the literal pool (the
 * gUnknown_03003340 word moves to slot 0) and it costs the `adds r6,r2,#0`
 * copy of the map-descriptor address -- measured this wave, both spellings
 * probed side by side. Same family as the wave-38 rule that anything after a
 * hoisted invariant was written by the loop optimiser and must not be authored.
 *
 * The `->rowOffset[y]` COMPONENT_REF is load-bearing: it is what keeps the
 * address association `(map + 0x417A) + y * 2` that the ROM has, with 0x417A
 * living in its own callee-saved register. Writing
 * `*(u16 *)(gUnknown_08499590 + 0x417A + y * 2)` reassociates to
 * `(map + y * 2) + 0x417A` (wave 34, W34-F).
 *
 * The three-level load chain at the guard (`ldr rN,=<word>; ldr r2,[rN];
 * ldr r1,[r2]`) is agbcc's own -fforce-addr copy of &gUnknown_08499590 -- the
 * ROM word at 0x08090934 -- supplying the middle level. The honest spelling
 * reproduces it; naming a `u8 **` pool word here would add a fourth level.
 * Both bounds are re-read every iteration, as c_0801F838 describes. */
void SetWorkingMapPlane(u8 *a1)
{
    int y;

    for (y = 0; y < gMap->height; y++)
        gUnknown_03003340[y] = a1 + gMap->rowOffset[y];
    gUnknown_084999C8->unk28 = gMap->width;
    gUnknown_084999C8->unk29 = gMap->height;
}
asm(".global sub_0801F92C\n.thumb_set sub_0801F92C, SetWorkingMapPlane\n");

/* A busy-wait sized by the gMap screen: the nested loop has an
 * EMPTY body and exists only to burn width * height iterations.
 *
 * The inner loop reads as a countdown (`subs r0,#1; cmp r0,#0; bne`) but the
 * source is the ascending `for (x = 0; x < width; x++)` of c_0801F838 with the
 * body removed -- check_dbra_loop reverses a counted loop whose index is unused
 * in the body, which is the wave-38 rule that a decrementing loop bottom is not
 * evidence of a decrementing source.
 *
 * The two bounds are read differently for the same reason: with no store in the
 * body nothing can alias, so LICM hoists the OUTER bound into a register
 * (`adds r2,r3,#0`) and hoists the width load that feeds the inner loop's
 * zero-trip guard, while the reversed inner loop's own trip-count init reloads
 * the width from memory each outer iteration. Both bounds are still written as
 * plain re-reads in the source, exactly as c_0801F838 has them; do not hoist
 * either into a local. */
void sub_0801F98C(void)
{
    int x;
    int y;

    for (y = 0; y < gMap->height; y++)
        for (x = 0; x < gMap->width; x++)
            ;
}
