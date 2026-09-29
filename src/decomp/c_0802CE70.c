#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CE70.
 * sub_0802CE70 @ 0x0802CE70
 */

/* OptionsMenu_Delete's sibling: the same u8 second argument stashed in the word
 * global gUnknown_030044A0 (the word `str` is what keeps PROMOTE_MODE's shift
 * pair live), then a two-argument call instead of a halfword store.
 *
 * This function is what corrected DefeatOtherTeamsAndEndMatch's prototype from `(int, int)` to
 * `(u8, u8)`. The ROM reads the u16 gUnknown_030033EC with a bare `ldrb`, which
 * is the byte load only a u8 parameter produces out of a halfword global; the
 * callee's own prologue (`lsls r0,#0x18; lsrs r0,#0x18` twice) says the same
 * thing directly. The old declaration survived because every other promoted
 * caller passes literal constants.
 */

void sub_0802CE70(int a1, u8 a2)
{
    gUnknown_030044A0 = a2;
    CloseTopMenu();
    DefeatOtherTeamsAndEndMatch(gUnknown_030033EC, 8);
}
