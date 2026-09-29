#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A028.
 * sub_0804A028 @ 0x0804A028
 */

void LanguageSelect_StartCursorScript(void)
{
    sub_080152EC(gUnknown_084C3824, 0);
}
asm(".global sub_0804A028\n.thumb_set sub_0804A028, LanguageSelect_StartCursorScript\n");
