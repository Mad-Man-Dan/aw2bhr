#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801296C.
 * sub_0801296C @ 0x0801296C, sub_0801297C @ 0x0801297C
 */

#include "hardware.h"

void EnableForcedBlank(void)
{
    gDispIo.disp_ct.forced_blank = 1;
}
asm(".global sub_0801296C\n.thumb_set sub_0801296C, EnableForcedBlank\n");

void DisableForcedBlank(void)
{
    gDispIo.disp_ct.forced_blank = 0;
}
asm(".global sub_0801297C\n.thumb_set sub_0801297C, DisableForcedBlank\n");
