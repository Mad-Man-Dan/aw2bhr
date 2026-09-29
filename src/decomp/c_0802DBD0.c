#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DBD0.
 * sub_0802DBD0 @ 0x0802DBD0, sub_0802DBE4 @ 0x0802DBE4
 */

/* Three statements. RestoreMapCursorPosition and DecrementMapLock take nothing, so the
 * SetInfoBoxMode(1) result cannot be flowing into either of them. */

void DeploymentScreen_Finish(void)
{
    SetInfoBoxMode(1);
    RestoreMapCursorPosition();
    DecrementMapLock();
}
asm(".global sub_0802DBD0\n.thumb_set sub_0802DBD0, DeploymentScreen_Finish\n");

/* The same one-line forwarder as the OptionsMenu_HelpVisualA group: 0xC9E is >255 so
 * agbcc has no `movs #imm8` for it and the pool word is forced by the VALUE
 * alone -- no symbol and no type is involved. `pop {r0}`, so void. */

void ShowUnitLimitMessage(void)
{
    StartCoSpeechScript(0xC9E, 0, 0);
}
asm(".global sub_0802DBE4\n.thumb_set sub_0802DBE4, ShowUnitLimitMessage\n");
