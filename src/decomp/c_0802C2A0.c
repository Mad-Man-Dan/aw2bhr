#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C2A0.
 * sub_0802C2A0 @ 0x0802C2A0
 */

/* The pool word is the symbol's ADDRESS, not its contents, so this compares a
 * pointer argument against the script itself -- the same object StartSaveConfirmScript
 * and EndSaveConfirmScript hand to StartEventScript / EndEventScript.
 */

bool8 IsSaveConfirmScript(const u8 *a)
{
    if (a == gUnknown_0849A8F0)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802C2A0\n.thumb_set sub_0802C2A0, IsSaveConfirmScript\n");
