#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807728C.
 * sub_0807728C @ 0x0807728C, sub_080772B8 @ 0x080772B8
 */

struct Unk080772B8
{
    /* 0x00 */ u8 filler_00[0x92];
    /* 0x92 */ u16 unk92[5][3];
};

void PutNumberTilesRightAligned(u16 *dst, int val)
{
    while (1) {
        *dst = val % 10 + 0x32;
        val /= 10;
        if (val == 0)
            break;
        dst--;
    }
}
asm(".global sub_0807728C\n.thumb_set sub_0807728C, PutNumberTilesRightAligned\n");

void WorldMapMissionInfo_DrawPropertyCounts(struct Unk080772B8 *p)
{
    u8 buf[8];
    int i;

    CountMapTilesOfTerrainKinds((s16)gUnknown_08615194[gUnknown_0202FDFC.unk0c].mapID,
                 gUnknown_086145C8, buf);

    for (i = 0; i <= 4; i++)
        PutNumberTilesRightAligned(&p->unk92[i][2], buf[i]);
}
asm(".global sub_080772B8\n.thumb_set sub_080772B8, WorldMapMissionInfo_DrawPropertyCounts\n");
