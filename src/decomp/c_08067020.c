#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067020.
 * sub_08067020 @ 0x08067020
 */

/* Teardown for the 0x08067 screen: one of two sound cues depending on
 * gUnknown_0202F200's mode flag, then the three stops and a window redraw. */
void MatchSetupScreen_Close(void)
{
    if (gUnknown_0202F200 != 0)
        PlayMusicOrSfx2(0x66);
    else
        PlayMusicOrSfx2(0x71);

    MatchSetupDismissRuleOptions(-2);
    sub_08065700();
    sub_08014878();

    FillTilemapRect(gBG0TilemapBuffer, 3, 0x11, 0x1a, 3, 0);
}
asm(".global sub_08067020\n.thumb_set sub_08067020, MatchSetupScreen_Close\n");
