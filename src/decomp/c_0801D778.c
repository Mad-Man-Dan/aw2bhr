#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D778.
 * sub_0801D778 @ 0x0801D778
 */

/* Arity is read off InitSpriteScript's prologue, not off this body -- forwarded
 * parameters cost no instruction here. See the note in
 * include/unknown-functions.h. */
int sub_0801D778(int a, int b, int c, int d)
{
    return InitSpriteScript(a, b, c, d, 0);
}
