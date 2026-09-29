#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802150C.
 * InitGameSettings @ 0x0802150C
 */

/* The between-maps reset: seventeen globals cleared or seeded, with four
 * subsystem resets interleaved at the points the ROM calls them.
 *
 * gUnknown_030033E4 and gUnknown_030033E0 are `struct Unk802C57C` pairs, which
 * is why each takes two `strh` off one pool word rather than two pool words.
 *
 * TWO zeros are materialised: r4 serves every halfword and word store, r5 only
 * the `strb` into the u8 gUnknown_030030F8 at the end. agbcc keeps a separate
 * QImode copy of the constant, so the second `movs` is not a spare statement.
 * The 1 stored into gUnknown_03003F3C is reused for gUnknown_03004080. */
void InitGameSettings(void)
{
    gUnknown_030033E4.unk00 = 0;
    gUnknown_030033E4.unk02 = 0;
    gUnknown_030033E0.unk00 = 0;
    gUnknown_030033E0.unk02 = 0;

    InitCursorInfoPanelPosition();

    gUnknown_030040F0 = 0;
    gUnknown_030044A0 = 0;

    RemoveMapLock();

    gUnknown_030040E4 = 0;
    gUnknown_030040E8 = 0;
    gUnknown_03003F60 = 0;
    gUnknown_03003F3C = 1;
    gUnknown_030032D8 = 5;
    gUnknown_03003334 = 0;
    gUnknown_030033EC = 0;
    gUnknown_03004080 = 1;
    gUnknown_030030F8 = 0;

    ClearAllUnits();
    ResetInventionRecords();
}

asm(".global sub_0802150C\n.thumb_set sub_0802150C, InitGameSettings\n");
