#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080591E4.
 * sub_080591E4 @ 0x080591E4, sub_08059464 @ 0x08059464
 */

/* AiAdvanceToward @ 0x080591E4, 640 bytes. MATCHED.
 *
 * The battle-cursor threat scan: walk every reachable cell, keep the one with
 * the lowest +0x2D5A threat value that passes the reachability and terrain
 * tests, and issue it as a AiPublishAction move. The sibling AiAdvanceTowardUnseeded at
 * 0x08059464 runs the same loop without the gUnknown_030013EC cursor-draw
 * callback, the +0x2D5A prefetch or the `== 100` seed.
 *
 * NOT a transcription twin of AiAdvanceTowardUnseeded despite tools/twin_pairs.py rating
 * them 0.538 -- 288 instructions against 240, with a four-instruction block
 * deleted, an 18-instruction run replaced by five, and different frame sizes.
 * What DID transfer between them is the whole loop body verbatim; each was
 * matched separately and neither derived from the other.
 *
 * Uses `gMap` (include/map.h), but the `SetWorkingMapPlane` setup calls must also
 * pass `gMap->danger` / `gMap->move` (the plane's own array member,
 * decays to `u8 *`), not `gUnknown_08499590 + offset` or a cast -- agbcc's
 * CSE only reuses a pointer load across identical symbols, and mixing in the
 * raw name anywhere forces a second pool load (see AiPickSafestReachableCell for the
 * fuller writeup of this).
 *
 * `mask` MUST BE A BYTE, and this was the entire residual: with `int mask` the
 * function is 640 bytes with every instruction in the right place, and only
 * `y` and the loop's `&gMap` pointer have r8 and sb swapped
 * (`mov r9,r4` / `mov r8,r3` instead of `mov r8,r4` / `mov sb,r3`, with the
 * three reads following). Narrowing the local changes nothing in the emitted
 * stream -- it lives in a stack slot at [sp,#0x10] either way, and THUMB's
 * lack of an SP-relative `ldrb` keeps the access a word `ldr` -- but it
 * changes the allocno and flips the two loop pseudos back. `u8` and plain
 * `char` both match; `u8` is used because gUnknown_085D5ABC's unk1d is
 * documented as a bit mask over the +0x3262 plane. Found by decomp-permuter in
 * 304 iterations, which is the wave-37 claim about the permuter holding:
 * order-right, slot-wrong is exactly its case.
 *
 * The same loop in AiAdvanceTowardUnseeded matched first try with `int mask`, because
 * that function has no pre-loop high-register allocnos competing for reuse. So
 * the type of a SPILLED local is a register-allocation lever even when it
 * costs zero instructions, and whether it matters depends on code OUTSIDE the
 * loop it affects.
 *
 * Three more things confirmed by compile_probe, recorded so no one re-spends a
 * round on them:
 *   - `gUnknown_03004784[1] >= gUnknown_030040D8->unk07[3] % 100` needs NO
 *     casts, even though it emits `bl __umodsi3` (not __modsi3), then
 *     `lsls #0x18; lsrs #0x18`, then `blo` (not `blt`). build_binary_op sets
 *     `shorten` for TRUNC_MOD_EXPR when the original left operand's type is
 *     unsigned, so shorten_binary_op redoes the modulo in `unsigned char`;
 *     shorten_compare then makes the comparison unsigned. Adding `(u8)` or
 *     `100u` breaks it.
 *   - the pre-loop `best` read comes out `ldrsb` here and `ldrb; lsl; asr` in
 *     AiAdvanceTowardUnseeded from the SAME `(s8)map->danger[...]` source: `ldrsb` needs
 *     a spare register for the zero index, and here the address landed in r2
 *     leaving r0 free while there it landed in r0.
 *   - the 5th argument to the gUnknown_030013EC indirect call is the literal 0
 *     and still comes out as `ldr r4,[sp,#0xc]` -- CSE substituting the
 *     equal-valued `bestY` slot, not a variable in the source.
 *
 * gUnknown_0816D950 / 0816D954 / 0816D958 are NOT globals: the ROM words hold
 * 0x08499590, 0x030040D8 and 0x03004784, so they are agbcc -fforce-addr
 * constants. The honest spelling reproduces all three and the promotion must
 * carry them -- data/promoted.json needs
 * "rodata": ["0x0816D950", "0x0816D954", "0x0816D958"].
 *
 * `bl _call_via_r4` is the ordinary ARMv4T indirect-call trampoline for
 * gUnknown_030013EC, whose five-int declaration in include/unknown-globals.h
 * matches the four register arguments plus one stack word here.
 */

/* AiAdvanceTowardUnseeded @ 0x08059464, 528 bytes. MATCHED (first attempt).
 *
 * The sibling of AiAdvanceToward at 0x080591E4: the same "walk every cell, keep
 * the lowest-threat reachable one, issue it as a AiPublishAction move" scan, minus
 * the gUnknown_030013EC cursor-draw callback, minus the +0x2D5A prefetch, and
 * minus the `gUnknown_03004784[1] == 100` special case that seeds `best` with
 * 0x7FFF. Its `void *` parameter is declared (unknown-functions.h) but never
 * read -- the ROM prologue does not touch r0.
 *
 * NOT a transcription twin of AiAdvanceToward -- see the note in that function's
 * work file. The two share this loop and nothing else structurally.
 *
 * gUnknown_0816D95C / 0816D960 / 0816D964 are NOT globals: the ROM words there
 * hold 0x03004784, 0x030040D8 and 0x08499590, so they are agbcc's own
 * -fforce-addr constants for gUnknown_03004784, gUnknown_030040D8 and
 * gUnknown_08499590. The honest spelling reproduces all three and the promotion
 * must carry the rodata words -- data/promoted.json needs
 * "rodata": ["0x0816D95C", "0x0816D960", "0x0816D964"].
 *
 * `gUnknown_03004784[1] >= gUnknown_030040D8->unk07[3] % 100` needs NO casts
 * and that is worth recording, because the emitted stream looks like it needs
 * three. It comes out as `bl __umodsi3` (not __modsi3), then `lsls #0x18;
 * lsrs #0x18`, then `blo` (not `blt`) -- all three from the C front end:
 * build_binary_op sets `shorten` for TRUNC_MOD_EXPR when the original left
 * operand's type is unsigned, so shorten_binary_op redoes the modulo in
 * `unsigned char` (hence the unsigned helper and the truncation back to a
 * byte), and shorten_compare then sees two unsigned narrow operands and makes
 * the comparison unsigned. Writing `(u8)(x % 100)` or `% 100u` gets none of it.
 *
 * x and y are `s16`, not `int`: every read of them goes through
 * `lsls #0x10; asrs #0x10` and the loop increment is the `+ 0x10000` on a
 * kept-shifted induction variable. `best` is `s16` too -- stores narrow with
 * `lsls #0x10; lsrs #0x10` (PROMOTE_MODE zero-extends) while reads
 * sign-extend.
 */

void AiAdvanceToward(void *a1)
{
    u16 *cur;
    s16 x;
    s16 y;
    s16 best;
    s16 bestX;
    s16 bestY;
    u8 mask;

    cur = a1;
    bestY = 0;
    mask = 0;

    SetWorkingMapPlane(gMap->danger);
    gUnknown_030013EC(cur[0], cur[1], gUnknown_030040D8->unk00, 0x78, 0);

    if (gUnknown_03004784[1] >= gUnknown_030040D8->unk07[3] % 100)
    {
        AiBuildThreatPlane();
        mask = gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d;
    }

    SetWorkingMapPlane(gMap->move);
    GenerateUnitMovementMap(gUnknown_030040D8);

    if (gUnknown_03004784[1] == 100)
        best = 0x7fff;
    else
        best = gMap->danger[gMap->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02];

    bestX = -1;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;
            if ((u8)gMap->danger[gMap->rowOffset[y] + x] > best)
                continue;
            if (gMap->dangerMask[gMap->rowOffset[y] + x] & mask)
                continue;
            if (!AiIsSettleCellOk(x, y))
                continue;
            if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != 0x20
             && (gMap->terrain[gMap->rowOffset[y] + x] & 0x1f) == 0xb)
                continue;
            best = gMap->danger[gMap->rowOffset[y] + x];
            bestX = x;
            bestY = y;
        }
    }

    if (bestX >= 0)
        AiPublishAction(bestX, bestY, 2, 0, 0);

    if (gUnknown_030045CC.unk00_1)
    {
        if (gUnknown_03004784[1] > gUnknown_030040D8->unk07[3] % 100)
            AiRetreat();
        AiFallbackMove();
    }
    AiFallbackMove();
}
asm(".global sub_080591E4\n.thumb_set sub_080591E4, AiAdvanceToward\n");

void AiAdvanceTowardUnseeded(void *a1)
{
    s16 x;
    s16 y;
    s16 best;
    s16 bestX;
    s16 bestY;
    int mask;

    bestY = 0;
    mask = 0;

    if (gUnknown_03004784[1] >= gUnknown_030040D8->unk07[3] % 100)
    {
        AiBuildThreatPlane();
        mask = gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d;
    }

    SetWorkingMapPlane(gMap->move);
    GenerateUnitMovementMap(gUnknown_030040D8);

    best = gMap->danger[gMap->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02];
    bestX = -1;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] < 0)
                continue;
            if ((u8)gMap->danger[gMap->rowOffset[y] + x] > best)
                continue;
            if (gMap->dangerMask[gMap->rowOffset[y] + x] & mask)
                continue;
            if (!AiIsSettleCellOk(x, y))
                continue;
            if ((gMap->terrain[gMap->rowOffset[y] + x] & 0x1f) == 0xb)
                continue;
            best = gMap->danger[gMap->rowOffset[y] + x];
            bestX = x;
            bestY = y;
        }
    }

    if (bestX >= 0)
        AiPublishAction(bestX, bestY, 2, 0, 0);

    if (gUnknown_030045CC.unk00_1)
    {
        if (gUnknown_03004784[1] > gUnknown_030040D8->unk07[3] % 100)
            AiRetreat();
        AiFallbackMove();
    }
    AiFallbackMove();
}
asm(".global sub_08059464\n.thumb_set sub_08059464, AiAdvanceTowardUnseeded\n");
