#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AFB0.
 * sub_0804AFB0 @ 0x0804AFB0
 */

/* +0x5c is the last element of struct Unk030044E0's unk2c[0x31]. The runtime
 * `adds r0, #0x5c` is the THUMB ldrb displacement limit, not an address being
 * taken.
 */
void NameEntry_SetFlagIfMode1(void)
{
    if (gUnknown_030044E0->unk2c[0x30] != 0)
        gUnknown_03002F1C = 1;
}
asm(".global sub_0804AFB0\n.thumb_set sub_0804AFB0, NameEntry_SetFlagIfMode1\n");
