#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044BB0.
 * sub_08044BB0 @ 0x08044BB0
 */

void BuildUnlockedCoGroupList(void)
{
    int i;
    int n;

    gUnknown_03005944 = 0;

    for (i = 0; i < 5; i++)
        gUnknown_03005948[i] = 0;

    n = 0;
    for (i = 0; gUnknown_084A08D0[i] != 0xff; i++)
    {
        if ((s8)gUnknown_084A08D0[i] < 0)
        {
            if (gUnknown_03005948[gUnknown_03005944] != 0)
            {
                gUnknown_03005958[gUnknown_03005944] = gUnknown_084A08D0[i] & 0x7f;
                gUnknown_03005944++;
            }
        }
        else if (IsCoSelectable(gUnknown_084A08D0[i]))
        {
            gUnknown_030058E0[n] = gUnknown_084A08D0[i];
            n++;
            gUnknown_03005948[gUnknown_03005944]++;
        }
    }
}
asm(".global sub_08044BB0\n.thumb_set sub_08044BB0, BuildUnlockedCoGroupList\n");
