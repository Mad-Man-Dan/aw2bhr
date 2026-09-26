#include "global.h"

/* PARKED at 568/572 bytes (-4), 58.6% identical.
 * Control flow, switch layout, calls, and pools are aligned. The ROM keeps
 * the force-address word for gUnknown_0300409C in r4 and creates a second
 * loop-local copy in r1, emitting `adds r1,r4,#0` at the swap-arm merge and
 * again at the do/while back edge. This draft reads [r4] directly at both
 * sites. Wave 61 tested binding `&gUnknown_0300409C` through a pointer local,
 * both for the whole outer loop and only after the swap merge. Both changed
 * the queue increment into a worse allocation and shortened the switch/tail;
 * neither created the two isolated copies. Prior 25,779-iteration permuter
 * and while-vs-do/while probes were also negative. */
void sub_0801F4B4(int a1, int a2, int a3, int a4, int a5)
{
    struct Unk300409C **cursor;

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

        cursor = &gUnknown_0300409C;
        if ((*cursor)->unk02 == 0)
            return;

        do {
            switch ((*cursor)->unk02) {
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
            (*cursor)++;
        } while ((*cursor)->unk02 != 0);
    }
}


