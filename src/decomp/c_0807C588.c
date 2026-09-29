#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C588.
 * sub_0807C588 @ 0x0807C588
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk807C588
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

void WarRoomMapSelected_0807C589(struct Unk807C588 *proc)
{
    int i;

    ResetAllPlayers();

    for (i = 0; i < GetMapArmyCount(gPlaySt.mapID); i++)
    {
        if (gUnknown_085C77A0[gPlaySt.mapID].unk3c[i] == 0xff)
            gPlayers[i + 1].aiControlled = 1;
        else
            gPlayers[i + 1].aiControlled = 2;
    }

    proc->unk58 = 0;
    proc->unk5c = 0;

    for (i = 0; i < 5; i++)
        gUnknown_03005910[i] = 0;
}

asm(".global sub_0807C588\n.thumb_set sub_0807C588, WarRoomMapSelected_0807C589\n");
