#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080212AC.
 * sub_080212AC @ 0x080212AC
 */

/* Runs one army's turn-start pass: first re-scores all 0x32 of its unit slots,
 * then walks the map and fires StampVisionByPlaneMask on every tile whose top three
 * terrain bits equal the army's gUnknown_084995F4 tag.
 *
 * gUnknown_08090960 in the asm is NOT a global: the ROM word at 0x08090960
 * holds 0x08499598, so it is agbcc's own -fforce-addr address constant for
 * gPlayers (the guard names it twice, the inner loop once, and only
 * the guard gets the word).
 *
 * The `ldrb` on gUnknown_084995FE is combine narrowing a halfword load to the
 * byte the `(u8)` cast keeps, not evidence of a byte table -- `faction * 2` is
 * CSE'd with gUnknown_084995F4's index and spilled to [sp,#8], which is what
 * proves both tables have a 2-byte stride.
 *
 * That table's declaration LOST its `const` for this function (see
 * include/unknown-globals.h): with `const`, agbcc proves the StampUnitVision call
 * cannot write it and hoists the element load out of the loop.  All seven
 * existing readers were re-verified and still match.
 *
 * `y << 16` living in r8 across the inner loop and the `asrs #0x10` that
 * unpacks it at the call are loop-optimiser output -- y is a plain int here. */
void StampArmyVision(u16 faction)
{
    int i;
    int x;
    int y;

    if (gPlayers[faction].aiControlled != 0 && gPlayers[faction].turnState != 0)
    {
        for (i = 0; i <= 0x32; i++)
            StampUnitVision((u8)(gUnknown_084995FE[faction] + i), 1);

        for (y = 0; y < gMap->height; y++)
        {
            for (x = 0; x < gMap->width; x++)
            {
                if ((gMap->terrain[
                         gMap->rowOffset[y] + x] & 0xE0)
                    == gUnknown_084995F4[faction])
                {
                    StampVisionByPlaneMask(x, y, 0, gPlayers[faction].turnState, 1, 0);
                }
            }
        }
    }
}
asm(".global sub_080212AC\n.thumb_set sub_080212AC, StampArmyVision\n");
