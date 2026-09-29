#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AFE8.
 * sub_0808AFE8 @ 0x0808AFE8
 */

int ProgramFlashSectorAndVerify(u16 a1, int a2)
{
    u8 i;
    int result;

    for (i = 0; i < 3; i++)
    {
        result = gUnknown_03005C74(a1, a2);

        if (result == 0)
        {
            result = VerifyFlashSector(a1, a2);

            if (result == 0)
                break;
        }
    }

    return result;
}
asm(".global sub_0808AFE8\n.thumb_set sub_0808AFE8, ProgramFlashSectorAndVerify\n");
