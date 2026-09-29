#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F57C.
 * sub_0802F57C @ 0x0802F57C
 */

/* The u16 twin of src/decomp/c_0802F4F4.c's s8 getter, on the volatile member
 * two bytes below it. ldrh with no shift pair is what makes it u16 rather
 * than s16, and the volatile qualifier already on unk04 costs nothing here. */
u16 SioGetLinkPhase(void)
{
    return gUnknown_0849B018->unk04;
}
asm(".global sub_0802F57C\n.thumb_set sub_0802F57C, SioGetLinkPhase\n");
