#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039188.
 * DrawMarkerSprites @ 0x08039188
 *
 * Not a Xenesis-documented name. The old sub_08039188 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "map.h"

/* Draws the overworld marker sprites. Walks gUnknown_0849D5F8's parallel
 * per-slot tables from the last slot (unk45) down. For every marker whose 16x16
 * box is on screen (sub_08039140) it puts a sprite at the camera-relative
 * position. The tile comes from gUnknown_0849D5C4 by what sub_080390CC and
 * sub_08039064 classify the slot as.
 *
 * Parked from wave 57 to wave 88 at +4. It matches unchanged against the current
 * headers, now that sub_08039140 is an old-style definition declared
 * `u8 sub_08039140();`. The caller no longer narrows `x * 16` for a u16
 * parameter, which is the conflict the park recorded. The draft's only other
 * change is the camera offsets read as gMap->scrollX / scrollY.
 */
void DrawMarkerSprites(void)
{
    s8 i;
    int x;
    int y;

    if (gUnknown_0849D5F8->unk45 == 0)
        return;

    for (i = gUnknown_0849D5F8->unk45; i >= 0; i--)
    {
        x = gUnknown_0849D5F8->unk20[i];
        y = gUnknown_0849D5F8->unk2c[i];

        if (sub_08039140(x * 16, y * 16, 0x10, 0x10) != 0)
        {
            PutSprite(3,
                      (x * 16 - gMap->scrollX) & 0x1FF,
                      (y * 16 - gMap->scrollY) & 0xFF,
                      gUnknown_0848B690,
                      0x3000 | gUnknown_0849D5C4[sub_080390CC(i)][sub_08039064(i)]);
        }
    }
}

asm(".global sub_08039188\n.thumb_set sub_08039188, DrawMarkerSprites\n");
