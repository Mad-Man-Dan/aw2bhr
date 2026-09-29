#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C2A0.
 * sub_0802C2A0 @ 0x0802C2A0
 */

/* The pool word is the symbol's ADDRESS, not its contents, so this compares a
 * pointer argument against the script itself -- the same object sub_0802C280
 * and sub_0802C290 hand to StartEventScript / EndEventScript.
 */

bool8 sub_0802C2A0(const u8 *a)
{
    if (a == gUnknown_0849A8F0)
        return TRUE;

    return FALSE;
}
