#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015E54.
 * sub_08015E54 @ 0x08015E54
 */

u8 SlotOp_Halt(u8 a)
{
    return 0;
}
asm(".global sub_08015E54\n.thumb_set sub_08015E54, SlotOp_Halt\n");
