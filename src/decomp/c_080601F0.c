#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080601F0.
 * sub_080601F0 @ 0x080601F0, sub_08060264 @ 0x08060264, sub_080602C4 @ 0x080602C4, sub_08060324 @ 0x08060324, sub_08060384 @ 0x08060384, sub_080603D4 @ 0x080603D4
 */

#include "hardware.h"

/* One arm of the 0x08060 cursor state machine: if the cell under the CURRENT
 * UNIT (gUnknown_030046C0.unk06 indexes gUnits, and the unit carries
 * its own column/row in unk02/unk03) is occupied on the gMap->unk234A plane, hand it
 * to ScrollCameraToKeepCellInView and advance to state 7; otherwise state 3.
 *
 * NO POINTER IS BOUND. The unit element is named twice and CSE gives it one
 * address -- and that is the whole difference between this and a candidate that
 * is 96.6% right. `u = &gUnits[i]` puts the table's DEREF after the
 * `* 12` (91.4%); adding `tbl = gUnits;` to pull the deref forward
 * fixes the order but then the declared local takes r1 where the ROM wants r0,
 * because a declared local's quantity is created at expand_decl and sorts ahead
 * of the unnamed product. Two levers that each fix half. Naming the element
 * twice needs neither: the deref lands first AND stays unnamed.
 *
 * The column is assigned inside the offset expression -- see c_08060264.c for
 * why that spelling and not a statement of its own. Row/tile arithmetic is
 * c_08001158.c's idiom with the plane at +0x234A. */
void AiExecutorCheckTargetUnitVisible(void)
{
    struct Map *map;
    int i;
    int x;
    int y;
    int off;

    map = gMap;
    i = gUnknown_030046C0.unk06;
    y = gUnits[i].y;
    off = map->rowOffset[y] + (x = gUnits[i].x);

    if (map->unk234A[off] != 0)
    {
        ScrollCameraToKeepCellInView(x, y);
        gUnknown_030046D4 = 0;
        gUnknown_030045D4 = 7;
    }
    else
        gUnknown_030045D4 = 3;
}
asm(".global sub_080601F0\n.thumb_set sub_080601F0, AiExecutorCheckTargetUnitVisible\n");

/* One arm of the 0x08060 cursor state machine: if the cursor's own cell
 * (gUnknown_030046C0.unk06/.unk07) is occupied on the gMap->unk234A plane, hand it to
 * ScrollCameraToKeepCellInView and advance to state 8; otherwise state 4.
 *
 * THE COLUMN IS ASSIGNED INSIDE THE OFFSET EXPRESSION. It has to be read after
 * the row-offset `ldrh` -- a statement of its own puts the `ldrb` two
 * instructions early -- but it also has to be a NAMED LOCAL, because
 * `ScrollCameraToKeepCellInView(gUnknown_030046C0.unk06, y)` sets up r1 before r0 while the ROM
 * sets r0 first. The embedded assignment is the only spelling that gets both.
 *
 * The cell is gMap->unk234A[gMap->rowOffset[y] + x]. */
void AiExecutorCheckTargetCellVisible(void)
{
    struct Map *map;
    int x;
    int y;
    int off;

    map = gMap;
    y = gUnknown_030046C0.unk07;
    off = map->rowOffset[y] + (x = gUnknown_030046C0.unk06);

    if (map->unk234A[off] != 0)
    {
        ScrollCameraToKeepCellInView(x, y);
        gUnknown_030046D4 = 0;
        gUnknown_030045D4 = 8;
    }
    else
        gUnknown_030045D4 = 4;
}
asm(".global sub_08060264\n.thumb_set sub_08060264, AiExecutorCheckTargetCellVisible\n");

/* AiExecutorCheckTargetCellVisible's twin, states 9 and 5. The ONLY structural difference is that
 * both cursor bytes are read before gUnknown_08499590 -- `ldrb r3, [r0, #6];
 * ldrb r4, [r0, #7]` off one pool word, ahead of the map pointer -- so the
 * column is bound first here and last there. Two functions of the same shape
 * that are not the same spelling; see c_08060264.c. */
void AiExecutorCheckMissileTargetVisible(void)
{
    struct Map *map;
    int x;
    int y;
    int off;

    x = gUnknown_030046C0.unk06;
    y = gUnknown_030046C0.unk07;
    map = gMap;
    off = map->rowOffset[y] + x;

    if (map->unk234A[off] != 0)
    {
        ScrollCameraToKeepCellInView(x, y);
        gUnknown_030046D4 = 0;
        gUnknown_030045D4 = 9;
    }
    else
        gUnknown_030045D4 = 5;
}
asm(".global sub_080602C4\n.thumb_set sub_080602C4, AiExecutorCheckMissileTargetVisible\n");

/* AiExecutorDwellOnTargetCell's variant that parks gUnknown_030033E4 on the CURRENT UNIT's
 * cell instead of the cursor's: gUnknown_030046C0.unk06 indexes
 * gUnits and unk02/unk03 are the unit's own column and row.
 *
 * THE UNIT POINTER MUST BE BOUND here, unlike in c_080601F0.c where naming the
 * element twice is what works. The difference is the store in between: `strh`
 * into gUnknown_030033E4 may alias the pointer global gUnits, so
 * agbcc rebuilds the whole subscript for the second member and the function
 * comes out 12 bytes long. c_080601F0.c has no store between its two reads. */
void AiExecutorDwellOnTargetUnit(void)
{
    struct Unit *u;

    u = &gUnits[gUnknown_030046C0.unk06];

    gUnknown_030033E4.unk00 = u->x;
    gUnknown_030033E4.unk02 = u->y;

    StepMapCursorAndDraw(2);

    gUnknown_030046D4++;

    if (gUnknown_030046D4 > 0x1e || (gpKeySt->held & 1))
        gUnknown_030045D4 = 3;
}
asm(".global sub_08060324\n.thumb_set sub_08060324, AiExecutorDwellOnTargetUnit\n");

/* A wait state of the 0x08060 cursor machine: it parks gUnknown_030033E4 on the
 * cursor's own cell, kicks StepMapCursorAndDraw, and leaves for state 4 after thirty
 * frames or on the A button, whichever comes first.
 *
 * gUnknown_030046D4 is VOLATILE and this function is what measures it: the ROM
 * re-reads it with a fresh `ldr` immediately after the `str` that increments
 * it, which a plain `int` never does -- gcc keeps the stored value in r0 and
 * the candidate comes out 4 bytes short. It is also `int` and not `u32`,
 * because the compare against 0x1e is `bgt`.
 *
 * gUnknown_030033E4 is a `struct Unk802C57C` pair, so both halves come off one
 * pool word; the two sources are u8 members widened by the `strh`. */
void AiExecutorDwellOnTargetCell(void)
{
    gUnknown_030033E4.unk00 = gUnknown_030046C0.unk06;
    gUnknown_030033E4.unk02 = gUnknown_030046C0.unk07;

    StepMapCursorAndDraw(2);

    gUnknown_030046D4++;

    if (gUnknown_030046D4 > 0x1e || (gpKeySt->held & 1))
        gUnknown_030045D4 = 4;
}
asm(".global sub_08060384\n.thumb_set sub_08060384, AiExecutorDwellOnTargetCell\n");

/* AiExecutorDwellOnTargetCell's twin, byte-for-byte except the state it leaves for (5 rather
 * than 4). See src/decomp/c_08060384.c for the gUnknown_030046D4 volatile and
 * signedness readings. */
void AiExecutorDwellOnMissileTarget(void)
{
    gUnknown_030033E4.unk00 = gUnknown_030046C0.unk06;
    gUnknown_030033E4.unk02 = gUnknown_030046C0.unk07;

    StepMapCursorAndDraw(2);

    gUnknown_030046D4++;

    if (gUnknown_030046D4 > 0x1e || (gpKeySt->held & 1))
        gUnknown_030045D4 = 5;
}
asm(".global sub_080603D4\n.thumb_set sub_080603D4, AiExecutorDwellOnMissileTarget\n");
