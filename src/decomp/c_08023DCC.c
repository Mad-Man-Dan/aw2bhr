#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023DCC.
 * sub_08023DCC @ 0x08023DCC, sub_08023E14 @ 0x08023E14, sub_08023E5C @ 0x08023E5C, sub_08023EA4 @ 0x08023EA4
 */

/* Family F043 (tools/families.py): four 72-byte wrappers that run two or three
 * redraw passes over the same four arguments. Four `lsls #0x10; lsrs #0x10`
 * pairs into four callee-saved registers is four genuine `u16` parameters --
 * unlike the F041 case, here the narrowing survives BECAUSE each value is used
 * two or three times across calls, and every callee re-reads it unconverted
 * (`adds rN, rM, #0`), which is only free when the declared parameter is
 * u16-compatible.
 *
 * RedrawMapColumnForScrollLeft and RedrawMapColumnForScrollRight are byte-for-byte the same function, as are
 * RedrawMapRowForScrollUp and RedrawMapRowForScrollDown -- same callees, same global, same
 * relocations. Two duplicated bodies, not four; the ROM really does contain
 * both copies.
 */
void RedrawMapColumnForScrollLeft(u16 a, u16 b, u16 c, u16 d)
{
    BlitMapColumn(a, b, c, d);
    RedrawUnitLayerColumn(a, b, c, d);

    if (gUnknown_03000559 == 1)
        RedrawRangeOverlayColumn(a, b, c, d);
}
asm(".global sub_08023DCC\n.thumb_set sub_08023DCC, RedrawMapColumnForScrollLeft\n");

/* Family F043 (tools/families.py): four 72-byte wrappers that run two or three
 * redraw passes over the same four arguments. Four `lsls #0x10; lsrs #0x10`
 * pairs into four callee-saved registers is four genuine `u16` parameters --
 * unlike the F041 case, here the narrowing survives BECAUSE each value is used
 * two or three times across calls, and every callee re-reads it unconverted
 * (`adds rN, rM, #0`), which is only free when the declared parameter is
 * u16-compatible.
 *
 * RedrawMapColumnForScrollLeft and RedrawMapColumnForScrollRight are byte-for-byte the same function, as are
 * RedrawMapRowForScrollUp and RedrawMapRowForScrollDown -- same callees, same global, same
 * relocations. Two duplicated bodies, not four; the ROM really does contain
 * both copies.
 */
void RedrawMapColumnForScrollRight(u16 a, u16 b, u16 c, u16 d)
{
    BlitMapColumn(a, b, c, d);
    RedrawUnitLayerColumn(a, b, c, d);

    if (gUnknown_03000559 == 1)
        RedrawRangeOverlayColumn(a, b, c, d);
}
asm(".global sub_08023E14\n.thumb_set sub_08023E14, RedrawMapColumnForScrollRight\n");

/* Family F043 (tools/families.py): four 72-byte wrappers that run two or three
 * redraw passes over the same four arguments. Four `lsls #0x10; lsrs #0x10`
 * pairs into four callee-saved registers is four genuine `u16` parameters --
 * unlike the F041 case, here the narrowing survives BECAUSE each value is used
 * two or three times across calls, and every callee re-reads it unconverted
 * (`adds rN, rM, #0`), which is only free when the declared parameter is
 * u16-compatible.
 *
 * RedrawMapColumnForScrollLeft and RedrawMapColumnForScrollRight are byte-for-byte the same function, as are
 * RedrawMapRowForScrollUp and RedrawMapRowForScrollDown -- same callees, same global, same
 * relocations. Two duplicated bodies, not four; the ROM really does contain
 * both copies.
 */
void RedrawMapRowForScrollUp(u16 a, u16 b, u16 c, u16 d)
{
    BlitMapRow(a, b, c, d);
    RedrawUnitLayerRow(a, b, c, d);

    if (gUnknown_03000559 == 1)
        RedrawRangeOverlayRow(a, b, c, d);
}
asm(".global sub_08023E5C\n.thumb_set sub_08023E5C, RedrawMapRowForScrollUp\n");

/* Family F043 (tools/families.py): four 72-byte wrappers that run two or three
 * redraw passes over the same four arguments. Four `lsls #0x10; lsrs #0x10`
 * pairs into four callee-saved registers is four genuine `u16` parameters --
 * unlike the F041 case, here the narrowing survives BECAUSE each value is used
 * two or three times across calls, and every callee re-reads it unconverted
 * (`adds rN, rM, #0`), which is only free when the declared parameter is
 * u16-compatible.
 *
 * RedrawMapColumnForScrollLeft and RedrawMapColumnForScrollRight are byte-for-byte the same function, as are
 * RedrawMapRowForScrollUp and RedrawMapRowForScrollDown -- same callees, same global, same
 * relocations. Two duplicated bodies, not four; the ROM really does contain
 * both copies.
 */
void RedrawMapRowForScrollDown(u16 a, u16 b, u16 c, u16 d)
{
    BlitMapRow(a, b, c, d);
    RedrawUnitLayerRow(a, b, c, d);

    if (gUnknown_03000559 == 1)
        RedrawRangeOverlayRow(a, b, c, d);
}
asm(".global sub_08023EA4\n.thumb_set sub_08023EA4, RedrawMapRowForScrollDown\n");
