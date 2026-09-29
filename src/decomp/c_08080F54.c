#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08080F54.
 * sub_08080F54 @ 0x08080F54
 */

/* SetMainMenuCarouselPosition @ 0x08080F54, 60 bytes, THUMB. Matched.
 *
 * Linear search of the six-byte permutation gUnknown_0861696C for the
 * parameter; on a hit gUnknown_03005934 gets DivRem(i + 4, 6), and when the
 * search runs out it gets the constant 4.
 *
 * The parameter is `int`, NOT the `u8` unknown-functions.h carried until
 * wave 44: there is no `lsls #0x18; lsrs #0x18` at the top of the function,
 * and PROMOTE_MODE puts that pair there unconditionally for a declared-narrow
 * parameter.
 *
 * The DivRem result MUST go through a local: the ROM loads
 * gUnknown_03005934's address AFTER the `bl DivRem` in the hit path and again
 * in the miss path, with only the `str` cross-jumped. Written as
 * `gUnknown_03005934 = DivRem(...)` the address load becomes partially
 * redundant with the miss path's and is hoisted into the loop preheader,
 * costing a second callee-saved register. See docs/agbcc-codegen.md.
 *
 * The two `.4byte` pool words sit INSIDE the function body, between the hit
 * and miss paths; that is ordinary literal placement.
 */
void SetMainMenuCarouselPosition(int a1)
{
    int i;

    for (i = 0; i <= 5; i++)
    {
        if (gUnknown_0861696C[i] == a1)
        {
            int r = DivRem(i + 4, 6);

            gUnknown_03005934 = r;
            return;
        }
    }

    gUnknown_03005934 = 4;
}
asm(".global sub_08080F54\n.thumb_set sub_08080F54, SetMainMenuCarouselPosition\n");
