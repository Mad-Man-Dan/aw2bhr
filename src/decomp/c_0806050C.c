#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806050C.
 * sub_0806050C @ 0x0806050C
 */

void AiExecutorFinishAfterDrop(void)
{
    int v = gUnknown_030040E4;

    if (v == 0)
    {
        if (gUnknown_030045E0[0] != NULL)
            EndMoveSlide(gUnknown_030045E0[0]);
        if (gUnknown_030045E0[1] != NULL)
            EndMoveSlide(gUnknown_030045E0[1]);
        CommitUnitMove();
        gUnknown_03004780 = 2;
        gUnknown_030045D4 = v;
    }
}
asm(".global sub_0806050C\n.thumb_set sub_0806050C, AiExecutorFinishAfterDrop\n");
