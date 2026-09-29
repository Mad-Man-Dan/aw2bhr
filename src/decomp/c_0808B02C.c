#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B02C.
 * sub_0808B02C @ 0x0808B02C
 */

/* ProgramFlashSectorAndVerify with a caller-supplied length: the same three-attempt
 * program-then-verify loop, with VerifyFlashSectorNBytes in place of VerifyFlashSector.
 *
 * `i` is a u8 (`adds; lsls #0x18; lsrs #0x18`) and both failure paths land on
 * the same increment, so it is one ordinary `for` with `result` declared ahead
 * of it -- the ROM's apparent read of an uninitialised `r3` when the loop is
 * not entered is just that, and must not be reproduced by hand. */
int ProgramFlashSectorAndVerifyNBytes(u16 a1, int a2, int a3)
{
    u8 i;
    int result;

    for (i = 0; i < 3; i++)
    {
        result = gUnknown_03005C74(a1, a2);

        if (result == 0)
        {
            result = VerifyFlashSectorNBytes(a1, a2, a3);

            if (result == 0)
                break;
        }
    }

    return result;
}
asm(".global sub_0808B02C\n.thumb_set sub_0808B02C, ProgramFlashSectorAndVerifyNBytes\n");
