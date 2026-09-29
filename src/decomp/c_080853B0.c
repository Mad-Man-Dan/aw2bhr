#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080853B0.
 * sub_080853B0 @ 0x080853B0
 */

/* Draws one 8-wide sprite per army slot, 24 pixels apart down the column at
 * x = 0x10. Two readings are not free:
 *   - The `+ 0x56` off a 0x3c-stride record is `[i + 1]`, not a member at 0x56:
 *     0x56 - 0x3c = 0x1a, so this is gPlayers[i + 1].unk1a (the same
 *     u8 palette/army key every other reader of that field uses) and slot 0 is
 *     skipped, which is why the loop counter starts at 0 while the record index
 *     does not.
 *   - The bound is re-evaluated every iteration, which is what puts the whole
 *     `?:` in the `for` condition rather than in a local: the ROM re-loads
 *     gPlaySt and re-runs the call at the loop bottom on each trip.
 *     Both arms return u8, hence the `lsls #0x18; lsrs #0x18` before the
 *     signed `cmp r4, r0; blt`.
 * `ldr r5, .LC` held across the loop is agbcc's -fforce-addr pool word for
 * &gPlaySt -- the ROM word the splitter calls gUnknown_081D93F8,
 * which dereferences to 0x03003FC0. */

void CoInfoScreen_DrawArmyIcons(void)
{
    int i;

    for (i = 0; i < (gPlaySt.gameMode == 2 ? GetMapArmyCount(gPlaySt.mapID) : GetLoadedMapArmyCount()); i++)
        DrawOamObject(gPlayers[i + 1].teamColor + 0x3d, i * 24 + 0x10, 8, 0, 1);
}
asm(".global sub_080853B0\n.thumb_set sub_080853B0, CoInfoScreen_DrawArmyIcons\n");
