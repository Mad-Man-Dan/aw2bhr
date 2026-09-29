#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080310D8.
 * sub_080310D8 @ 0x080310D8, sub_08031128 @ 0x08031128, sub_0803117C @ 0x0803117C
 */

/* One of three identical resets that differ only in the mode value they park in
 * gUnknown_0849B018->unk00 (1 here, 3 in sub_08031128, 2 in sub_0803117C).
 *
 * That store is what proved unk00 VOLATILE: a plain `= 1` on a non-volatile
 * member is one `strb`, and the ROM has a dead `ldrb` in front of it. See the
 * member's comment in unknown-globals.h.
 *
 * The four zeroes go to gPlaySt.aiControlled[1..4], not to four scalars --
 * the ROM walks the address up by one from +0x39, which is what consecutive
 * elements of the byte array already declared there give. */
void sub_080310D8(void)
{
    InitVersusPlayState();

    gPlaySt.savingEnabled = 1;
    gPlaySt.aiControlled[1] = 0;
    gPlaySt.aiControlled[2] = 0;
    gPlaySt.aiControlled[3] = 0;
    gPlaySt.aiControlled[4] = 0;
    gPlaySt.fog = 1;
    gPlaySt.coPowersEnabled = 1;
    gPlaySt.weather = 0;
    gPlaySt.randomWeatherOn = 1;

    gUnknown_0849B018->unk00 = 1;
    gUnknown_0849B060->unk02 = 1;

    LinkLobbyLoadGraphics();
}

/* sub_080310D8 with mode 3. See that function for why unk00 is volatile and
 * why the four zeroes are unk38[1..4]. The mode value needs its own `movs`
 * here because 3 is not the 1 already live for the other stores. */
void sub_08031128(void)
{
    InitVersusPlayState();

    gPlaySt.savingEnabled = 1;
    gPlaySt.aiControlled[1] = 0;
    gPlaySt.aiControlled[2] = 0;
    gPlaySt.aiControlled[3] = 0;
    gPlaySt.aiControlled[4] = 0;
    gPlaySt.fog = 1;
    gPlaySt.coPowersEnabled = 1;
    gPlaySt.weather = 0;
    gPlaySt.randomWeatherOn = 1;

    gUnknown_0849B018->unk00 = 3;
    gUnknown_0849B060->unk02 = 1;

    LinkLobbyLoadGraphics();
}

/* sub_080310D8 with mode 2. See that function for why unk00 is volatile and
 * why the four zeroes are unk38[1..4]. */
void sub_0803117C(void)
{
    InitVersusPlayState();

    gPlaySt.savingEnabled = 1;
    gPlaySt.aiControlled[1] = 0;
    gPlaySt.aiControlled[2] = 0;
    gPlaySt.aiControlled[3] = 0;
    gPlaySt.aiControlled[4] = 0;
    gPlaySt.fog = 1;
    gPlaySt.coPowersEnabled = 1;
    gPlaySt.weather = 0;
    gPlaySt.randomWeatherOn = 1;

    gUnknown_0849B018->unk00 = 2;
    gUnknown_0849B060->unk02 = 1;

    LinkLobbyLoadGraphics();
}
