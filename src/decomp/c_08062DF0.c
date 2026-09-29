#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062DF0.
 * sub_08062DF0 @ 0x08062DF0
 */

#include "hardware.h"
/* MATCHED. The map-editor debug overlay.
 *
 * gUnknown_0816DB40/50/5C/68/74/80 are printf FORMAT STRINGS, not data objects:
 * baserom.gba holds "UNIT  C0  C1", "PSQ(%1d)", "ASQ(%1d)", "USQ(%1d)",
 * "CPT(%1d)" and "(%1d)" there, and DebugPrintf's third parameter is
 * `const char *`. Their ROM spacing (0x10, 0xc, 0xc, 0xc, 0xc) is exactly
 * agbcc's 4-byte alignment of 13, 9, 9, 9, 9 and 6 bytes, which confirms both
 * the reading and each extent. They are NOT the -fforce-addr pool words at
 * 0x0816DB00..0x0816DB1C: the ADDRESS is the argument, never dereferenced.
 *
 * The honest spelling is a string literal -- matched c_080281F0.c spells its
 * "SNOW:%s" siblings that way -- and it produces a .rodata section BYTE-
 * IDENTICAL to ROM 0x0816DB40..0x0816DB86 (verified against baserom.gba), at
 * 98.9% with the five string pool words as the only differing bytes. It is
 * rejected by tools/trymatch.py:306, which requires `len(.rodata) % 4 == 0`
 * and this section is 0x46. Naming the symbols is the compromise; see
 * docs/agbcc-codegen.md.
 *
 * DebugPrintf is VARARGS, and that is what explains the FIRST call leaving r3
 * holding gUnknown_030040D8: a three-argument call never writes r3, so the
 * leftover is not an argument. A fixed-arity prototype cannot produce it.
 *
 * gUnknown_030040D8 keeps its declared `struct Unk030040D8 *` -- over a hundred
 * promoted files reach it that way -- so the assignment uses c_0802E4B4.c's
 * cast idiom. The three extractions off byte 9 (`lsls #0x1d/lsrs #0x1d`,
 * `lsls #0x1a/lsrs #0x1d`, `lsrs #6`) are a 3/3/2 bitfield view; c_0805B814.c's
 * struct Unk40D8Bits covers the SAME byte with a 6/2 split, so the tag is named
 * differently here on purpose -- do not merge them.
 *
 * The map planes go through the `struct Map` cast of c_08000BF8.c. Plain `u8 *`
 * arithmetic on gUnknown_08499590 reassociates the 0x417A / 0x12 constants past
 * the index and does not match.
 */

struct Unk8062DF0Bits
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};
#define MAP gMap
#define BITS(p) ((struct Unk8062DF0Bits *)(p))

void sub_08062DF0(void)
{
    struct Unk030040D8 *a;
    struct Unk030040D8 *b;

    a = 0;
    b = 0;

    HandleMoveMapCursor();
    HandleGameMapCursorInput();
    HandleMoveCameraWithMapCursor(4);
    StepMapCursorAndDraw(0);

    if ((MAP->unk10 & 0xF) == 0)
    {
        if (gpKeySt->pressed == 1)
        {
            gUnknown_03004780 = gUnknown_030045DC;
            SetMapLayersDefault();
            ClearBg0Tilemap();
            BG_EnableSyncBG0();
            return;
        }

        if ((gpKeySt->held & 4)
            && gMap->unit[MAP->rowOffset[gUnknown_030033E4.unk02]
                            + gUnknown_030033E4.unk00] != 0)
        {
            gUnknown_03003F38 = gMap->unit[MAP->rowOffset[gUnknown_030033E4.unk02]
                                             + gUnknown_030033E4.unk00];
            gUnknown_030040D8 = (struct Unk030040D8 *)&gUnits[gUnknown_03003F38];

            if (gUnknown_030040D8->unk07[0] != 0)
                a = (struct Unk030040D8 *)&gUnits[gUnknown_030040D8->unk07[0]];

            if (gUnknown_030040D8->unk07[1] != 0)
                b = (struct Unk030040D8 *)&gUnits[gUnknown_030040D8->unk07[1]];

            DebugPrintf(2, 4, gUnknown_0816DB40);
            DebugPrintf(2, 5, gUnknown_0816DB50, BITS(gUnknown_030040D8)->unk09_0);
            DebugPrintf(2, 6, gUnknown_0816DB5C, BITS(gUnknown_030040D8)->unk09_3);
            DebugPrintf(2, 7, gUnknown_0816DB68, BITS(gUnknown_030040D8)->unk09_6);
            DebugPrintf(2, 8, gUnknown_0816DB74, gUnknown_030040D8->unk07[4]);

            if (a != 0)
            {
                DebugPrintf(9, 5, gUnknown_0816DB80, BITS(a)->unk09_0);
                DebugPrintf(9, 6, gUnknown_0816DB80, BITS(a)->unk09_3);
                DebugPrintf(9, 7, gUnknown_0816DB80, BITS(a)->unk09_6);
                DebugPrintf(9, 8, gUnknown_0816DB80, a->unk07[4]);
            }

            if (b != 0)
            {
                DebugPrintf(0xE, 5, gUnknown_0816DB80, BITS(b)->unk09_0);
                DebugPrintf(0xE, 6, gUnknown_0816DB80, BITS(b)->unk09_3);
                DebugPrintf(0xE, 7, gUnknown_0816DB80, BITS(b)->unk09_6);
                DebugPrintf(0xE, 8, gUnknown_0816DB80, b->unk07[4]);
            }
        }
    }

    RefreshMapCursorInfoPanel();
    SetInfoBoxMode(0);
}
