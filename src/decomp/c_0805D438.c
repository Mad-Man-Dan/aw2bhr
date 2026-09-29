#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805D438.
 * sub_0805D438 @ 0x0805D438
 */

#include "map.h"

/*
 * AiRunNextWorklistUnit -- AI: run one step of the current army's unit list.
 *
 * gUnknown_030046B0 walks a list of unit indices. 0 ends the list and 0x40
 * is a special entry with no unit. Nothing happens while gUnknown_030044D8
 * is set.
 *   1. End of list: set gUnknown_03004780 to 1 and stop.
 *   2. Unit entry: make it the active unit (gUnknown_03003F38 index,
 *      gUnknown_030040D8 pointer, gUnknown_03004784 its class row). Skip a
 *      unit with flag bit 0 set or type 0. Otherwise save its position in
 *      gUnknown_03003F24, clear some AI state, lift it off the map's unit
 *      layer, set flag bit 2, store a random 0..99 in byte 0x0A and call
 *      AiUpdateNeededPropertyKind.
 *   3. If sub_08071908(gUnknown_03004680) returns 0, call AiRunResupplyOrRepairMission (unit
 *      entries only) and then the handler in gUnknown_03004778.
 *   4. If gUnknown_030046C0.unk13 is still 0, put the unit back on the map,
 *      clear flag bit 2 and advance the list. Otherwise advance the list and
 *      set gUnknown_03004780 to 3.
 *
 * Why the C looks odd:
 *   - The class row is written as byte arithmetic, `base + (type * 12 + 4)`.
 *     The rows are 12 bytes, and the header's struct for this table is
 *     bigger. Indexing a 12-byte struct adds the 4 at a different point.
 *
 * Built with the compiler's global common-subexpression pass turned off
 * (-O2 -fno-gcse), through its entry in data/compiler-overrides.json. With
 * that pass on, the compiler shares the address loads of three globals
 * across the call and the function comes out 28 bytes longer.
 */
void AiRunNextWorklistUnit(void)
{
    if (gUnknown_030044D8 != 0)
        return;

    if (*gUnknown_030046B0 == 0)
    {
        gUnknown_03004780 = 1;
        return;
    }

    if (*gUnknown_030046B0 != 0x40)
    {
        gUnknown_03003F38 = *gUnknown_030046B0;
        gUnknown_030040D8 = (struct Unk030040D8 *)&gUnknown_08499594[gUnknown_03003F38];
        gUnknown_03004784 = (u8 *)gUnknown_085766E0 + (gUnknown_030040D8->unk00 * 12 + 4);

        if ((gUnknown_030040D8->unk01 & 1) || gUnknown_030040D8->unk00 == 0)
        {
            gUnknown_030046B0++;
            return;
        }

        gUnknown_03003F24.pos.unk00 = gUnknown_030040D8->unk02;
        gUnknown_03003F24.pos.unk02 = gUnknown_030040D8->unk03;

        gUnknown_030045CC.unk00_0 = 0;
        gUnknown_030045CC.unk00_1 = 0;

        gUnknown_030046C0.unk00 = 0;
        gUnknown_030046C0.unk13 = 0;
        gUnknown_030046C0.unk06 = 0;
        gUnknown_030046C0.unk07 = 0;

        ((struct Map *)gUnknown_08499590)->unit[((struct Map *)gUnknown_08499590)->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02] = 0;

        gUnknown_030040D8->unk01 |= 4;
        gUnknown_030040D8->unk07[3] = sub_080129E0() % 100;
        AiUpdateNeededPropertyKind();
    }
    else
    {
        gUnknown_030046C0.unk13 = 0;
    }

    if (sub_08071908(gUnknown_03004680) == 0)
    {
        if (*gUnknown_030046B0 != 0x40)
            AiRunResupplyOrRepairMission();

        gUnknown_030045CC.unk00_1 = 0;
        gUnknown_03004778();
    }

    if (gUnknown_030046C0.unk13 == 0)
    {
        if (*gUnknown_030046B0 != 0x40)
            ((struct Map *)gUnknown_08499590)->unit[((struct Map *)gUnknown_08499590)->rowOffset[gUnknown_030040D8->unk03] + gUnknown_030040D8->unk02] = gUnknown_03003F38;

        gUnknown_030040D8->unk01 &= ~4;
        gUnknown_030046B0++;
    }
    else
    {
        gUnknown_030046B0++;
        gUnknown_03004780 = 3;
    }
}
asm(".global sub_0805D438\n.thumb_set sub_0805D438, AiRunNextWorklistUnit\n");
