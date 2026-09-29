#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D67C.
 * sub_0802D67C @ 0x0802D67C
 */

void BuildDeploymentList(u8 mask)
{
    u8 i;
    u8 n;
    u8 id;

    n = 0;
    i = 0;

    while (gUnknown_081BA054[i] != -1)
    {
        id = gUnknown_081BA054[i];

        if (gUnknown_085D5ABC[id].deployLocation & mask)
        {
            if (id != 8 || IsNeotanksUnlocked(gUnknown_030033EC))
            {
                if (gPlayers[gUnknown_030033EC].funds
                        < GetUnitCostWithCoBonus(gUnknown_030033EC, id) * 10)
                {
                    u8 *p = &gUnknown_02023830[n * 4];

                    p[1] = 2;
                }
                else
                {
                    u8 *p = &gUnknown_02023830[n * 4];

                    p[1] = 0;
                }

                gUnknown_02023830[n++ * 4] = id;
            }
        }
        i++;
    }

    gUnknown_0300055A = n;
    gUnknown_02023830[n * 4] = 0;
}
asm(".global sub_0802D67C\n.thumb_set sub_0802D67C, BuildDeploymentList\n");
