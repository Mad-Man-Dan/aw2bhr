#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035490.
 * CalcRandomWeatherChances @ 0x08035490
 */

/* Rebuilds the two gUnknown_03004490 counters from the four armies'
 * gPlayers records, then zeroes the per-army flag beside them.
 *
 * gPlayers arrives through agbcc's own -fforce-addr word at
 * 0x08090EA0, whose ROM content is 0x08499598 (dereferenced in baserom.gba --
 * its neighbour 0x08090EA4 holds 0x08499590 instead, a DIFFERENT global, so
 * the two words are not interchangeable). Naming the global honestly gives the
 * ROM's three-level `ldr rN,=<word>; ldr r0,[rN]; ldr r1,[r0]`, and the
 * `(i * 16 - i) * 4` synthesis is just i * 0x3c, the record size.
 *
 * The +3 flag array is reached through a STRUCT POINTER, not as
 * `gUnknown_03004490[i + 3]`, and that is 2 bytes plus a register: only the
 * struct spelling lets LICM hoist the member offset into its own
 * `adds r6, r4, #3` ahead of the loop. Written as an index, gcc computes
 * `i + 3` inside the loop instead; written as `*(gUnknown_03004490 + 3 + i)`
 * it folds the +3 into the SYMBOL, which -fforce-addr then pools as a second
 * `.rodata` word the ROM does not have. See the hoisted-member-offset note in
 * docs/agbcc-codegen.md. */
struct Unk35490Counts
{
    /* 0x00 */ u8 filler_00[0x03];
    /* 0x03 */ u8 unk03[5];
};

void CalcRandomWeatherChances(void)
{
    u8 i;

    gUnknown_03004490[1] = gUnknown_03004490[2] = 6;

    for (i = 1; i < 5; i++)
    {
        if (gPlayers[i].aiControlled != 0)
        {
            gUnknown_03004490[1]--;
            gUnknown_03004490[2]--;
            gUnknown_03004490[1] += GetPlayerCoRainBringerPercent(i);
            gUnknown_03004490[2] += GetPlayerCoSnowBringerPercent(i);
        }

        ((struct Unk35490Counts *)gUnknown_03004490)->unk03[i] = 0;
    }

    LoadWeatherData();
}

asm(".global sub_08035490\n.thumb_set sub_08035490, CalcRandomWeatherChances\n");
