#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023EEC.
 * sub_08023EEC @ 0x08023EEC
 */

void UpdateMapDisplay(void)
{
    if ((gMap->scrollX < (s16)gMap->unk08
      && (gMap->scrollX >> 4) != ((s16)gMap->unk08 >> 4))
     || (gMap->scrollX > (s16)gMap->unk08
      && (gMap->scrollX >> 4) != (((s16)gMap->unk08 - 1) >> 4)))
    {
        if (gMap->scrollX < (s16)gMap->unk08)
            RedrawMapColumnForScrollLeft((gMap->scrollX >> 4) - gMap->camX,
                         (gMap->scrollY >> 4) - gMap->camY,
                         gMap->scrollX >> 4,
                         gMap->scrollY >> 4);
        else
            RedrawMapColumnForScrollRight(((gMap->scrollX >> 4) + 0xf) - gMap->camX,
                         (gMap->scrollY >> 4) - gMap->camY,
                         (gMap->scrollX >> 4) + 0xf,
                         gMap->scrollY >> 4);

        BG_EnableSyncBG1();
        BG_EnableSyncBG2();
        BG_EnableSyncBG3();

        if (gUnknown_03000559 == 1)
            BG_EnableSyncBG0();
    }

    if ((gMap->scrollY < (s16)gMap->unk0a
      && (gMap->scrollY >> 4) != ((s16)gMap->unk0a >> 4))
     || (gMap->scrollY > (s16)gMap->unk0a
      && (gMap->scrollY >> 4) != (((s16)gMap->unk0a - 1) >> 4)))
    {
        if (gMap->scrollY < (s16)gMap->unk0a)
            RedrawMapRowForScrollUp((gMap->scrollX >> 4) - gMap->camX,
                         (gMap->scrollY >> 4) - gMap->camY,
                         gMap->scrollX >> 4,
                         gMap->scrollY >> 4);
        else
            RedrawMapRowForScrollDown((gMap->scrollX >> 4) - gMap->camX,
                         ((gMap->scrollY >> 4) + 0xa) - gMap->camY,
                         gMap->scrollX >> 4,
                         (gMap->scrollY >> 4) + 0xa);

        BG_EnableSyncBG1();
        BG_EnableSyncBG2();
        BG_EnableSyncBG3();

        if (gUnknown_03000559 == 1)
            BG_EnableSyncBG0();
    }

    gMap->unk08 = gMap->scrollX;
    gMap->unk0a = gMap->scrollY;
}
asm(".global sub_08023EEC\n.thumb_set sub_08023EEC, UpdateMapDisplay\n");
