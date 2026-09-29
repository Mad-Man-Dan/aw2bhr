#include "global.h"

/*
 * sub_0801F4B4 -- walk outward over the map from the cell (a1, a2), round by round.
 *
 * It keeps two queues inside gUnknown_084999C8 (one at +0x2c, one at +0x5a4)
 * and swaps them after each round: one is read while the next round is written
 * to the other. The unk02 field of each queue entry selects which of the four
 * neighbours (sub_0801F6F0 with directions 2 to 5) to visit; the exact meaning
 * of the field is not worked out yet. The walk ends when a round leaves the
 * queue empty.
 *
 * Why the C looks odd: `pp` is only used in the do/while condition. Reading
 * the queue cursor through a pointer to it there, instead of naming the global,
 * changes how the compiler keeps the global's address in a register.
 */
void sub_0801F4B4(int a1, int a2, int a3, int a4, int a5)
{
    struct Unk300409C **pp;

    gUnknown_030040E0 = 0;
    gUnknown_03003FBC = 0;
    gUnknown_03003F64 = (u8 *)gUnknown_084999C8 + 0x2c;
    gUnknown_0300409C = (struct Unk300409C *)((u8 *)gUnknown_084999C8 + 0x5a4);
    gUnknown_084999C8->unk20 = a4;
    gUnknown_084999C8->unk22 = a5;
    gUnknown_084999C8->unk24[0] = 1;
    gUnknown_084999C8->unk24[1] = 2;
    gUnknown_084999C8->unk24[2] = 4;
    gUnknown_084999C8->unk24[3] = 8;
    gUnknown_084999C8->unk2a = gPlayers[gUnknown_03004480].unk2c;
    sub_0801F838(0xff);
    sub_0801F888(a3);
    gUnknown_03003F64[0] = a1;
    gUnknown_03003F64[1] = a2;
    gUnknown_03003F64[2] = 1;
    gUnknown_03003F64[3] = 0;
    gUnknown_03003340[a2][a1] = 0;
    gUnknown_03003F64 += 4;
    gUnknown_03003F64[2] = 0;

    for (;;) {
        if (gUnknown_03003FBC == 0) {
            gUnknown_03003F64 = (u8 *)gUnknown_084999C8 + 0x5a4;
            gUnknown_0300409C = (struct Unk300409C *)((u8 *)gUnknown_084999C8 + 0x2c);
            gUnknown_03003FBC = 1;
            gUnknown_030040E0 = 0;
        } else {
            gUnknown_03003F64 = (u8 *)gUnknown_084999C8 + 0x2c;
            gUnknown_0300409C = (struct Unk300409C *)((u8 *)gUnknown_084999C8 + 0x5a4);
            gUnknown_03003FBC = 0;
            gUnknown_030040E0 = 0;
        }

        if (gUnknown_0300409C->unk02 == 0)
            return;

        pp = &gUnknown_0300409C;
        do {
            switch (gUnknown_0300409C->unk02) {
            case 1:
                sub_0801F6F0(2, 0, 0xff);
                sub_0801F6F0(3, 0, 1);
                sub_0801F6F0(4, 0xff, 0);
                sub_0801F6F0(5, 1, 0);
                break;
            case 2:
                sub_0801F6F0(2, 0, 0xff);
                sub_0801F6F0(4, 0xff, 0);
                sub_0801F6F0(5, 1, 0);
                break;
            case 3:
                sub_0801F6F0(3, 0, 1);
                sub_0801F6F0(4, 0xff, 0);
                sub_0801F6F0(5, 1, 0);
                break;
            case 4:
                sub_0801F6F0(2, 0, 0xff);
                sub_0801F6F0(3, 0, 1);
                sub_0801F6F0(4, 0xff, 0);
                break;
            case 5:
                sub_0801F6F0(2, 0, 0xff);
                sub_0801F6F0(3, 0, 1);
                sub_0801F6F0(5, 1, 0);
                break;
            }
            gUnknown_03003F64[2] = 0;
            gUnknown_0300409C++;
        } while ((*pp)->unk02 != 0);
    }
}




