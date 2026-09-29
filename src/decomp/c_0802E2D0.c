#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E2D0.
 * sub_0802E2D0 @ 0x0802E2D0
 */

/* Wave 60 (W60-C). First derivation ever attempted on this function -- the
 * previous .c was start_function's `return 0;` stub, which the delta screen
 * scored 0.0% / pool 0/15 and which read like a catastrophic miss.
 *
 * PROMOTION NEEDS THE POOL WORDS PLACED:
 *     "rodata": ["0x08090C30", "0x08090C34"]
 * Both are agbcc's own -fforce-addr address-constant words (0x08090C30 holds
 * &gUnknown_03003F38, 0x08090C34 holds &gUnknown_030040D8), not globals; the
 * honest spelling reproduces the ROM's load chains exactly. This is the wave-21
 * same-address case, not a park reason.
 *
 * TWO facts settled here that were not in any note before:
 *
 * 1. gUnknown_03003340's rows are written through an `(s8 *)` view. The store
 *    at +0x40C is `movs r2,#1; rsbs r2,r2,#0; adds r1,r2,#0; strb r1,[r0]` --
 *    THREE instructions plus a copy to build -1 in SImode. Spelled on the
 *    declared `u8 *` row type, `= -1` folds at tree level to the byte constant
 *    255 and agbcc emits one `movs r1,#0xff`. src/decomp/c_0801FE68.c found
 *    this first and its header states it exactly; the same `((s8 *)row)[x] = -1`
 *    spelling is used here. The shared declaration is NOT changed.
 *      What IS new is the size of the lever. That one constant was worth EIGHT
 *    register assignments: before the s8 cast, eight separate `mov rLow, rHigh`
 *    scratch picks disagreed with the ROM and NOTHING else in 484 bytes did;
 *    every one of them snapped into place when the -1 became three
 *    instructions. One extra live value re-seeds the whole allocation, so a
 *    diff that is "one constant plus a scatter of register names" is ONE fact
 *    -- do not chase the scratch picks individually.
 *
 * 2. sub_0801FE68's CALLER passes an argument its CALLEE ignores, and this does
 *    NOT need an edit to c_0801FE68.c. That file is byte-matched as
 *    `void sub_0801FE68(void)` and its body reads nothing from r0; the
 *    unknown-functions.h note asking for its signature to be "fixed" is wrong
 *    and has been corrected there. This call site really is
 *    `movs r0,#0x40; bl sub_0801FE68`, so MapCursor_OnPressB's own translation unit
 *    saw a declaration taking an argument -- a cross-TU prototype disagreement
 *    that agbcc cannot see and that costs nothing at either end. The file-local
 *    `void sub_0801FE68(int);` below reproduces it. LEAVE c_0801FE68.c ALONE.
 *
 * The rest is the wave-57 (W57-A) callee survey in unknown-functions.h, which
 * was right about all six signatures; they are declared file-locally rather
 * than in the header because the promoted definitions in src/decomp/ own them.
 * The casts on gUnknown_030040D8 are because struct Unk030040D8 and
 * struct Unit are the same object under two names (see the note on
 * Unk030040D8.unk01 in unknown-globals.h); they cost nothing.
 *
 * The map planes are reached as MEMBERS of `gMap` (include/map.h) and INLINE
 * rather than through a `map` local, which is the W34-F rule that
 * c_08003DC4.c records; that is what gives `(p + K) + idx` rather than
 * `(p + idx) + K`. Every use in the function -- including the
 * `MarkInventionFireArea`/`SetWorkingMapPlane` setup calls -- must name `gMap`, since
 * agbcc's CSE only reuses a pointer load across identical symbols (see
 * AiPickSafestReachableCell for the fuller writeup). */

struct Unk0803E9F8;
void SetMapLayersRangeBehindUnits(void);
void StartUnitsTranslucentPeek(void);
void PaintUnitAttackRange(s16, s16, struct Unit *);
int MarkInventionFireArea(struct Unk0803E9F8 *, u8 *, u8, u8);
int IsDirectFireUnitArmed(struct Unit *);
int IsIndirectFireUnitArmed(struct Unit *);
void sub_0801FE68(int);

u8 MapCursor_OnPressB(s16 x, s16 y)
{
    struct Unk02028360 *unit;
    u8 a;
    u8 b;

    unit = FindInventionAt(x, y);

    if (unit != NULL
     && (u8)MarkInventionFireArea((struct Unk0803E9F8 *)unit,
                         gMap->move, 0xFF, 0))
    {
        SetMapLayersRangeBehindUnits();
    }
    else
    {
        gUnknown_03003F38 = gMap->unit[
            gMap->rowOffset[y] + x];
        gUnknown_030040D8 =
            (struct Unk030040D8 *)&gUnits[gUnknown_03003F38];

        if (gMap->unit[
                gMap->rowOffset[y] + x] == 0)
        {
            StartUnitsTranslucentPeek();
            return 1;
        }

        a = IsDirectFireUnitArmed((struct Unit *)gUnknown_030040D8);
        b = IsIndirectFireUnitArmed((struct Unit *)gUnknown_030040D8);

        if (a == 0 && b == 0)
        {
            PlayMusicOrSfx2(0x68);
            return 0;
        }

        SetWorkingMapPlane(gMap->move);
        CreateMoveSlideForActiveUnit(gUnknown_030040D8);
        SetMapLayersRangeBehindUnits();
        RebuildMapUnitLayers();

        if (a)
        {
            gUnknown_03004480 = (gUnknown_03003F38 >> 6) + 1;
            GenerateUnitMovementMap(gUnknown_030040D8);
            gUnknown_03004480 = gUnknown_030033EC;
            ((s8 *)gUnknown_03003340[y])[x] = 0;
            sub_0801FE68(0x40);
            ((s8 *)gUnknown_03003340[y])[x] = -1;
        }

        if (b)
        {
            if (a == 0)
                FillMovementMap(0xFF);

            PaintUnitAttackRange(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                         (struct Unit *)gUnknown_030040D8);

            if (a)
            {
                ((s8 *)gUnknown_03003340[y])[x - 1] = 0;
                ((s8 *)gUnknown_03003340[y])[x + 1] = 0;
                ((s8 *)gUnknown_03003340[y - 1])[x] = 0;
                ((s8 *)gUnknown_03003340[y + 1])[x] = 0;
            }
        }
    }

    ShowRangeOverlay((u16)x, (u16)y, 1);
    gUnknown_03003334 = 6;
    PlayMusicOrSfx2(0x69);
    return 1;
}
asm(".global sub_0802E2D0\n.thumb_set sub_0802E2D0, MapCursor_OnPressB\n");
