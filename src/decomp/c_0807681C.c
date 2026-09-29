#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807681C.
 * sub_0807681C @ 0x0807681C, sub_08076858 @ 0x08076858
 */

void PaintClearedWorldMapSections(void)
{
    struct Unk0202FDFC *s;
    u8 *p;
    int i;

    for (i = 0, s = &gUnknown_0202FDFC, p = s->unk12; i <= 0x29; i++) {
        u8 v = *p;

        if (v != 0 && (v & 2) != 0) {
            v &= 0x7f;
            v &= 0xfe;
            *p = v;
            ColorWorldMapSection(i);
        }

        p++;
    }
}
asm(".global sub_0807681C\n.thumb_set sub_0807681C, PaintClearedWorldMapSections\n");

void RestoreWorldMapMarkers2(void)
{
    int i;

    for (i = 0; i <= 0x29; i++) {
        u8 v = gUnknown_0202FDFC.unk12[i];

        if (v != 0 && (v & 1) != 0)
            AddWorldMapMarker(i);
    }
}
asm(".global sub_08076858\n.thumb_set sub_08076858, RestoreWorldMapMarkers2\n");
