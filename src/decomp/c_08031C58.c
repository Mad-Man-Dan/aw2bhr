#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031C58.
 * sub_08031C58 @ 0x08031C58
 */

/* Draws a 0..100 percentage as three strips: a fixed prefix, the tens digit
 * when non-zero, and a units strip whose x steps by 8 per ten.
 *
 * The tens digit is an `int` with an explicit `(u16)` cast, not a `u16` local.
 * A u16 local makes gcc keep a second copy of it across the four calls -- an
 * extra `adds r5,r4,#0` and an extra pushed register, four bytes over. */
void LinkDrawTransferPercent(void)
{
    int n;

    n = (u16)(gUnknown_0849B060->unk0a / 10);

    PutOamHi(0x66, 0x38, gUnknown_084C145E, 0x5010);

    if (n != 0)
        PutOamHi(0x6e, 0x38, gUnknown_084C170C[n], 0x5010);

    if (n != 10)
        PutOamHi(n * 8 + 0x6e, 0x38, gUnknown_084C178C[10 - n], 0x5010);

    PutOamHi(0xbe, 0x38, gUnknown_084C1466, 0x5010);
}
asm(".global sub_08031C58\n.thumb_set sub_08031C58, LinkDrawTransferPercent\n");
