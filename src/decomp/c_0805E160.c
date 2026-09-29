#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E160.
 * sub_0805E160 @ 0x0805E160
 */

/* Wave 52, W52-D.  MATCHED (332/332, relocs resolve to the same addresses).
 *
 * gUnknown_0816DA5C / _0816DA60 / _0816DA64 are NOT globals -- the ROM words
 * there hold 0x08499590, 0x030040D8 and 0x030013EC, so they are agbcc's own
 * -fforce-addr constants for gUnknown_08499590, gUnknown_030040D8 and
 * gUnknown_030013EC.  The honest spelling reproduces all three and the
 * promotion must carry the three rodata words:
 *   "rodata": ["0x0816DA5C", "0x0816DA60", "0x0816DA64"]
 *
 * THE LOOP IS A GOTO LOOP, and that is the whole residual.  Written as a
 * `do { } while (r != 1)` the shape is otherwise byte-exact but LICM builds a
 * preheader out of three invariants -- the pooled 0x270F, `add rN,sp,#4` and
 * gUnknown_03004730's base -- which costs a third high callee-saved register
 * (the ROM pushes only r6/r7 for r8/sb) and pushes gUnknown_030013EC's
 * -fforce-addr pointer out of r5.  A goto loop has no NOTE_INSN_LOOP_BEG, LICM
 * never runs, and all of it comes back at once.  This is exactly the
 * measurement recorded on AiMoveTowardLandingCell (wave 51, W51-E), which is this
 * function's twin: same scratch cell, same 0x270F seed, same
 * gUnknown_03003340 / GetUnitMovementBudget / AiAdvanceToward tail.
 *
 * _call_via_r4 here is a FIVE-argument indirect call, not a four-argument one:
 * r4 is callee-saved, so the register index is not an arity readout (W46-D).
 *
 * The `(x & 0xc0) != 0` arm is written as the ELSE so that AiPlanLoadedLander lands
 * after the body; an early `return` puts that block before the body instead.
 *
 * struct Unk5A514Cell is repeated here rather than declared in a shared header
 * because src/decomp/c_0805A268.c, c_0805A514.c and c_0805A744.c all define it
 * file-locally; AiListEmbarkBoundUnits's promoted definition takes it, so the cast on
 * gUnknown_03003F20 is what keeps this unit in agreement with that definition.
 * See the note at AiPickSupplyWard in include/unknown-functions.h.
 *
 * Uses `gMap` (include/map.h) for the +0x12/+0x417A reads, but the
 * `SetWorkingMapPlane` setup call must pass `gMap->move` (the plane's own array
 * member, decays to `u8 *`), not `gUnknown_08499590 + 0x2852` or a cast --
 * agbcc's CSE only reuses a pointer load across identical symbols, so mixing
 * in the raw name anywhere forces a second pool load and breaks the match
 * (see AiPickSafestReachableCell for the fuller writeup of this). */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};
void AiListEmbarkBoundUnits(struct Unk5A514Cell *);
struct CellXY
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};

void AiDeliberateLander(void)
{
    struct CellXY pos;
    struct Unk03003338 *list;
    u8 r;

    list = gUnknown_03003F20;
    SetWorkingMapPlane(gMap->move);

    if ((gUnknown_030040D8->unk07[2] & 0xc0) == 0)
    {
        gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                          gUnknown_030040D8->unk00, 0x78, 1);
        MapMarkHalo(0x79);
        AiListEmbarkBoundUnits((struct Unk5A514Cell *)list);
    loop:
        pos.x = 0x270F;
        AiPopLastNearestCandidate(list, (u16 *)&pos);
        if (pos.x == 0x270F)
        {
            AiMoveLanderToNearestPort();
            return;
        }
        gUnknown_03004730[gMap->unit[gMap->rowOffset[pos.y] + pos.x] & 0x3f]++;
        r = AiPickShoalParkBeside(pos.x, pos.y, (u16 *)&pos);
        if (r != 1)
            goto loop;

        gUnknown_030013EC(pos.x, pos.y, gUnknown_030040D8->unk00, 0x78, 1);

        if ((s8)gUnknown_03003340[gUnknown_030040D8->unk03][gUnknown_030040D8->unk02] > GetUnitMovementBudget((struct Unit *)gUnknown_030040D8))
            AiAdvanceToward(&pos);
        else
            AiPublishAction(pos.x, pos.y, 2, 0, 0);
    }
    else
    {
        AiPlanLoadedLander();
    }
}
asm(".global sub_0805E160\n.thumb_set sub_0805E160, AiDeliberateLander\n");
