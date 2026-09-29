#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804542C.
 * sub_0804542C @ 0x0804542C
 */

void CoPowerOverlayFixed_Loop(void)
{
    gUnknown_03001418 -= 0x14;
    gUnknown_03001FF8 += 0xe;
}
asm(".global sub_0804542C\n.thumb_set sub_0804542C, CoPowerOverlayFixed_Loop\n");
