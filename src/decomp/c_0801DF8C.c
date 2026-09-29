#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DF8C.
 * sub_0801DF8C @ 0x0801DF8C
 */

u16 *GetOamShadow(void)
{
    return gUnknown_03002520;
}
asm(".global sub_0801DF8C\n.thumb_set sub_0801DF8C, GetOamShadow\n");
