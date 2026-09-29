#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080485DC.
 * sub_080485DC @ 0x080485DC
 */

/* Remember the blob and immediately run it. StartEventScript's result is discarded
 * -- pop {r0}; bx r0 overwrites r0 with the return address, which is the
 * spelling agbcc uses only for a void function.
 *
 * The argument is passed on unchanged, so its type comes entirely from
 * StartEventScript's const u8 *; ->unk850 takes the same type for the same
 * reason. */
void ShopScreen_StartMessage(const u8 *script)
{
    gUnknown_084C30F8->unk850 = script;
    StartEventScript(script);
}
asm(".global sub_080485DC\n.thumb_set sub_080485DC, ShopScreen_StartMessage\n");
