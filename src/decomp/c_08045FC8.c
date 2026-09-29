#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045FC8.
 * sub_08045FC8 @ 0x08045FC8
 */

/* Draws the army list: for each army 1..GetLoadedMapArmyCount() that is not defeated,
 * its team-colour sprite (teamColor + 0x3d) at x 8, y 0x30 + 16 * army; then
 * two fixed sprites. */
void DrawIntelStatusArmyIcons(void)
{
    u16 i;

    for (i = 1; i <= GetLoadedMapArmyCount(); i++)
    {
        /* Row y is 0x30 + 16 * i, spelled (i * 2 + 6) * 8 so that it is not
         * folded into the shift that scales the player index. */
        if (gPlayers[i].defeated == 0)
            sub_0801F34C(gPlayers[i].teamColor + 0x3d, 8, (i * 2 + 6) * 8, 0, 0);
    }

    sub_0801F34C(2, 8, 0x10, 0, 0);
    sub_0801F34C(0xa9, 0x5f, 0x30, 0, 0);
}
asm(".global sub_08045FC8\n.thumb_set sub_08045FC8, DrawIntelStatusArmyIcons\n");
