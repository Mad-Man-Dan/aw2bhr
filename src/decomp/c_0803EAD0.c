#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803EAD0.
 * sub_0803EAD0 @ 0x0803EAD0, sub_0803EB40 @ 0x0803EB40
 */

void InventionTurn_TickCountdowns(void)
{
    struct Unk02028360 *p;

    for (p = gUnknown_02028360; p->unk02_6 != 0; p++)
    {
        switch (p->unk02_6)
        {
        case 2:
            if (gUnknown_030033EC == 1)
                p->unk06--;
            break;
        case 1:
        case 3:
        case 4:
        case 5:
            if (gPlayers[gUnknown_030033EC].teamColor == 5)
                p->unk06--;
            break;
        }
    }
}
asm(".global sub_0803EAD0\n.thumb_set sub_0803EAD0, InventionTurn_TickCountdowns\n");

void InventionTurn_ReloadCountdowns(void)
{
    struct Unk02028360 *p;

    for (p = gUnknown_02028360; p->unk02_6 != 0; p++)
    {
        switch (p->unk02_6)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (p->unk06 == 0)
                p->unk06 = p->unk02_a;
            break;
        }
    }
}
asm(".global sub_0803EB40\n.thumb_set sub_0803EB40, InventionTurn_ReloadCountdowns\n");
