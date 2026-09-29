#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080358C4.
 * sub_080358C4 @ 0x080358C4
 */

/* Clamps the gUnknown_08499590 camera origin (+0x04, +0x06) so the point
 * (a1, a2) stays inside a 0x20..0xc0 by 0x20..0x70 window of the screen, then
 * calls UpdateMapBgScroll to re-scroll. 10000 is the "no change on this axis"
 * sentinel, and when BOTH axes are still 10000 the function returns without
 * storing anything.
 *
 * The parameters are s16 and the prototype was retyped to match -- see the
 * note in unknown-functions.h. The entry `lsls #0x10; lsrs #0x10` on the
 * second parameter is PROMOTE_MODE, not a cast: agbcc zero-extends a sub-word
 * parameter at entry whatever its signedness, and every use sign-extends. The
 * first parameter shows only the sign-extending pair because its use is in the
 * entry block, where combine folds the two together.
 *
 * The two clamp sentinels are u16 LOCALS: every write is `lsls #0x10;
 * lsrs #0x10` and every read `lsls #0x10; asrs #0x10`.
 *
 * `lsls #0x14; lsrs #0x10` on (extent - 0xf) is `(u16)((extent - 0xf) * 16)`,
 * a MULTIPLY and not a shift -- shorten_binary_op folds the u16 truncation
 * into MULT_EXPR but not into a shift. The same subexpression feeds the
 * comparison as a plain `lsls #4`, which is why it appears twice.
 *
 * gUnknown_08499590 is reached through agbcc's own -fforce-addr word at
 * 0x08090EA4 (content 0x08499590, dereferenced in baserom.gba -- note its
 * neighbour 0x08090EA0 holds 0x08499598 instead, so the two are NOT
 * interchangeable). Naming the global honestly reproduces the ROM's
 * three-level `ldr rN,=<word>; ldr r0,[rN]; ldr r1,[r0]`, and the reload after
 * each two-arm clamp is the merge-point reload, not a second variable. */
void ScrollCameraToKeepPixelInView(s16 a1, s16 a2)
{
    u16 nx;
    u16 ny;
    int dx;
    int dy;

    nx = 10000;
    ny = 10000;

    dx = a1 - gMap->scrollX;
    if (dx <= 0x1f)
        nx = a1 - 0x20;
    if (dx > 0xc0)
        nx = a1 - 0xc0;

    dy = a2 - gMap->scrollY;
    if (dy <= 0x1f)
        ny = a2 - 0x20;
    if (dy > 0x70)
        ny = a2 - 0x70;

    if ((s16)nx == 10000)
    {
        if ((s16)ny == (s16)nx)
            return;
        nx = gMap->scrollX;
    }
    else if ((s16)nx < 0)
        nx = 0;
    else if ((s16)nx > (gMap->width - 0xf) * 16)
        nx = (gMap->width - 0xf) * 16;

    if ((s16)ny == 10000)
        ny = gMap->scrollY;
    else if ((s16)ny < 0)
        ny = 0;
    else if ((s16)ny > (gMap->height - 0xa) * 16)
        ny = (gMap->height - 0xa) * 16;

    gMap->scrollX = nx;
    gMap->scrollY = ny;

    UpdateMapBgScroll();
}
asm(".global sub_080358C4\n.thumb_set sub_080358C4, ScrollCameraToKeepPixelInView\n");
