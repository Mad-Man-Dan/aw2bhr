#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E718.
 * sub_0805E718 @ 0x0805E718
 */

void sub_0805E718(void)
{
    struct Unk03003338 *p;

    sub_08062474();
    if (AiListAttackCandidates(AiPrepareAttackReach()) != 0)
    {
        p = sub_0805878C();
        if (p != NULL)
        {
            if ((p->unk00 & 0xff00) == 0)
                sub_0805D648(p->unk04, p->unk06, 4, p->unk00, 0);
            else
                sub_0805D648(p->unk04, p->unk06, 5, p->unk00 >> 8, p->unk00);
        }
    }
}
