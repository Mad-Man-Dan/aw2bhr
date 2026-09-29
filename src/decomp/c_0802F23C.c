#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F23C.
 * sub_0802F23C @ 0x0802F23C
 */

void LinkResetKeySync(void)
{
    gUnknown_0849B01C->unk00 = 0;
    gUnknown_0849B01C->unk02 = 0;
    gUnknown_0849B01C->unk04 = 1;
    gUnknown_0849B01C->unk05 = 0;
    gUnknown_0849B01C->unk06 = 0x5fff;
    gUnknown_0849B01C->unk210 = 0xffff;
    gUnknown_0849B01C->unk212 = 0;
}
asm(".global sub_0802F23C\n.thumb_set sub_0802F23C, LinkResetKeySync\n");
