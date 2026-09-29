#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802966C.
 * sub_0802966C @ 0x0802966C
 */

#include "hardware.h"

/* The two `gUnknown_03003100.pos.unkNN + (s8)table[...]` statements have to be
 * spelled with the global FIRST. Written the other way round the code is the
 * same size and every instruction is the same, but agbcc materialises
 * gUnknown_03003100's address after the table index instead of before it, and
 * the four pool words come out in a different order. Operand order of a `+`
 * decides which addend's address is CSEd into a register first.
 */
void DropCellPicker_Loop(void)
{
    struct Unit *unit;
    u8 r;

    if (gpKeySt->repeated & (DPAD_RIGHT | DPAD_UP))
        DropCellPicker_SelectNext();

    if (gpKeySt->repeated & (DPAD_LEFT | DPAD_DOWN))
        DropCellPicker_SelectPrevious();

    gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00
        + (s8)gUnknown_0849A06C[gUnknown_03001470[gUnknown_03001FBC].unk20 * 5 + 1];
    gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02
        + (s8)gUnknown_0849A06C[gUnknown_03001470[gUnknown_03001FBC].unk20 * 5 + 2];

    StepMapCursorAndDraw(1);

    if (gpKeySt->pressed & 2)
    {
        EndSlotScriptAt(gUnknown_03001FBC);
        DecrementMapLock();
        sub_0802D558();
        gUnknown_030033E4.unk00 = gUnknown_03003F24.pos.unk00;
        gUnknown_030033E4.unk02 = gUnknown_03003F24.pos.unk02;
    }
    else if (gpKeySt->pressed & 1)
    {
        gUnknown_03003F40 = gUnknown_03001470[gUnknown_03001FBC].unk20;

        unit = &gUnits[gUnknown_030040D8->unk07[
                   gUnknown_03001470[gUnknown_03001FBC].unk22]];

        gUnknown_030033E8[gUnknown_03000558] =
            gUnknown_03001470[gUnknown_03001FBC].unk20 + 1;

        gUnknown_03003110[0] =
            gUnknown_0849A06C[gUnknown_03001470[gUnknown_03001FBC].unk20 * 5 + 3];
        gUnknown_03003110[1] =
            gUnknown_0849A06C[gUnknown_03001470[gUnknown_03001FBC].unk20 * 5 + 4];

        r = TruncatePathAtHiddenEnemy((s16)gUnknown_03003100.pos.unk00,
                         (s16)gUnknown_03003100.pos.unk02, gUnknown_03003110, -1);

        if (r == 1)
        {
            gUnknown_03001470[gUnknown_03001FBC].unk24 = 1;
            gUnknown_030033E8[gUnknown_03000558] = 5;
        }

        CreateMoveSlideWithPath(gUnknown_03003100.pos.unk00, gUnknown_03003100.pos.unk02,
                     (((struct Unit *)gUnknown_030040D8 - gUnits) & 0xc0) >> 6,
                     unit->type, gUnknown_03003110);
        ClearSlotScriptCallback(gUnknown_03001FBC);
        sub_08029868(unit->type);
    }
}
asm(".global sub_0802966C\n.thumb_set sub_0802966C, DropCellPicker_Loop\n");
