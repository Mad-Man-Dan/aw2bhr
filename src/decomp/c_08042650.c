#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08042650.
 * sub_08042650 @ 0x08042650
 */

/* MATCHED (wave 66, W66-B), 532/532 bytes. Separate block-scoped volatile
 * reads of the map pointer keep the two cell-address chains independent;
 * struct Map field access for the row table and the terrain/unk234A planes
 * preserves `(map + field) + index`. Binding `&gUnknown_030040D8`, then its
 * pointee, emits both pointer loads before `n << 3` and retains the outer
 * pointer for the ROM's later reload. Promotion needs .rodata words
 * 0x08091354, 0x08091358 and 0x0809135C.
 *
 * SETTLED HERE, and none of it should be re-derived:
 *   - gUnknown_030040D8->unk04 is a 7-BIT BITFIELD. ApplyCaptureProgress reads it
 *     twice off ONE `ldrb` with two different extractions -- `movs r0,#0x7f;
 *     ands r0,r3` where it is only tested against zero, and `lsls r0,r3,#0x19;
 *     lsrs r0,r0,#0x19` where the value is used. No plain-u8 spelling gives
 *     both (fold() collapses `((u32)x << 25) >> 25` to the AND at both sites,
 *     because it runs before bitfield extraction is expanded). Declared in
 *     include/unknown-globals.h; the exact 532-byte size and the matching
 *     `ldrb r0,[r0,#5]` confirm the layout is unchanged.
 *   - the `<< 25 >> 25` / `* 0x55555555` / `rsbs` / `asrs` chain is an
 *     EXACT_DIV of a pointer difference, not a magic-number division:
 *     `(struct Unit *)gUnknown_030040D8 - gUnits`. gcc 2.x's
 *     expand_divmod multiplies by invert_mod2n(d >> post_shift) and shifts
 *     AFTERWARDS, so post_shift = 2 for the 12-byte record, and the extra
 *     6 in `asrs #8` is the caller's own `>> 6`. Both spellings appear here:
 *     `((p - base) & 0xc0) >> 6` (asrs #2, mask, asrs #6) and `(p - base) >> 6`
 *     (asrs #8), exactly as src/decomp/c_0802966C.c already had them.
 *   - four callees were promoted but undeclared and are now in
 *     unknown-functions.h; StartCaptureAnimation is derived from this call site alone
 *     (five arguments, the `str r3,[sp]` being the fifth, and void).
 *   - gUnknown_03003100 is read BOTH ways in one function: `.pos` (u16, the
 *     `ldrh`s feeding address arithmetic and StartCaptureAnimation) and `.spos` (s16,
 *     the `movs rI,#0; ldrsh` pair feeding CapturePropertyAt). The union already
 *     models this; do not pick one view for the whole function.
 *   - `t` is a `u8` local, not an `int`: `t >> 5` is `lsrs`. That is the
 *     OPPOSITE of the neighbouring MapEventCond_Army1UnitAtX7Y2, whose `>> 6` is `asrs` and
 *     needs an `int`; check the shift before copying that note.
 *   - `t & 0x1f` must be written INLINE at all three comparison sites. Bound
 *     to a `terr` local it hoists above the `gPlaySt.unk09` test;
 *     inline, CSE keeps it in r3 exactly where the ROM has it.
 */
void ApplyCaptureProgress(void)
{
    u8 t;
    int n;
    int v;
    struct Unk030040D8 **unitp;
    struct Unk030040D8 *unit;

    if (gUnknown_030040D8->unk04 != 0)
        n = Div(gUnknown_030040D8->unk04 - 1, 10) + 1;
    else
        n = 0;

    n = Div(n * GetPlayerCoCaptureRate(gUnknown_030033EC), 100);

    {
        struct Map *map;
        map = *(struct Map *volatile *)&gMap;
        t = map->terrain[
                map
                    ->rowOffset[gUnknown_03003100.pos.unk02]
                + gUnknown_03003100.pos.unk00];
    }
    ResetCaptureProgressIfMoved();
    RecordUnitActionCell();

    n += gUnknown_030040D8->unk05 >> 3;
    if (n > 0x13)
        n = 0x14;

    if (gPlaySt.fog == 0
        || (gPlayers[gUnknown_030033EC].turnState & 2) != 0)
        goto do_body;

    {
        struct Map *map;
        map = *(struct Map *volatile *)&gMap;
        if (map->unk234A[
                map
                    ->rowOffset[gUnknown_03003100.pos.unk02]
                + gUnknown_03003100.pos.unk00] == 0)
            goto after_body;
    }

do_body:
    {
       if (gPlaySt.animOpts == 1 || (t & 0x1f) == 8
            || (t & 0x1f) == 0x14)
            StartCaptureAnimation(gUnknown_03003100.pos.unk00,
                         gUnknown_03003100.pos.unk02,
                         gUnknown_030040D8->unk05 >> 3, n,
                         ((((struct Unit *)gUnknown_030040D8
                            - gUnits) & 0xc0) >> 6) + 1);
        else if (n > 0x13)
            PlayCaptureCompleteSound(((((struct Unit *)gUnknown_030040D8
                            - gUnits) & 0xc0) >> 6) + 1, t & 0x1f);
    }
after_body:
    unitp = &gUnknown_030040D8;
    unit = *unitp;
    v = n << 3;
    unit->unk05 = (unit->unk05 & 7) | v;

    if ((gUnknown_030040D8->unk05 >> 3) > 0x13)
    {
        AwardCapturePoints((((struct Unit *)gUnknown_030040D8
                       - gUnits) >> 6) + 1, t >> 5);
        gUnknown_030040D8->unk05 &= 7;
        if ((t & 0x1f) == 8 || (t & 0x1f) == 0x14)
            gPlayers[t >> 5].killOnEndTurn = 1;
        CapturePropertyAt(gUnknown_03003100.spos.unk00,
                     gUnknown_03003100.spos.unk02);
    }

    ParkMapState();
}
asm(".global sub_08042650\n.thumb_set sub_08042650, ApplyCaptureProgress\n");
