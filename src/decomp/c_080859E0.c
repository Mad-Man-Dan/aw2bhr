#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080859E0.
 * sub_080859E0 @ 0x080859E0
 */

void CoInfoPopup_Init(u16 *p)
{
    p[0x32] = 0;
}
asm(".global sub_080859E0\n.thumb_set sub_080859E0, CoInfoPopup_Init\n");
