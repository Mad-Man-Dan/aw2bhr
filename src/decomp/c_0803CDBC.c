#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CDBC.
 * sub_0803CDBC @ 0x0803CDBC, sub_0803CE28 @ 0x0803CE28
 */

#include "hardware.h"

int DrawDesignRoomSlotPreview(int a1, int a2, u8 a3)
{
    u8 *p;

    p = gUnknown_02000000;
    if (IsSaveSlotInvalid(a3 + 5) != 0)
        return 0;
    ReadSaveSlot(a3 + 5, p);
    SetLoadedMapBlob(p);
    SnapshotTeamColorsFromRecord((struct Unk3D6FC *)p);
    DrawMapPreviewToBg((void *)(0x06000000 + gUnknown_03002B6C.bits.chr_block * 0x4000),
                 1, 0, a1, a2, 5);
    return 1;
}
asm(".global sub_0803CDBC\n.thumb_set sub_0803CDBC, DrawDesignRoomSlotPreview\n");

void DrawDesignRoomMapPreview(int a1, int a2)
{
    u8 *p;

    p = gUnknown_02000000;
    sub_0803CFA4(gUnknown_0809113C, p, 1);
    SetLoadedMapBlob(p);
    SnapshotTeamColorsFromPlayers();
    ShowMapPreview((int)(0x06000000 + gUnknown_03002B6C.bits.chr_block * 0x4000),
                 (int)(gBG0TilemapBuffer + (a2 * 32 + a1)), 1, 5);
    sub_08013AD4(0);
}
asm(".global sub_0803CE28\n.thumb_set sub_0803CE28, DrawDesignRoomMapPreview\n");
