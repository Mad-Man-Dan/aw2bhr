#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080315D0.
 * sub_080315D0 @ 0x080315D0
 */

void sub_080315D0(void)
{
    DrawWindowBackgroundOnBg2(10, 5, 13, 10);
    LinkScreenSetupWindow(0);
}
