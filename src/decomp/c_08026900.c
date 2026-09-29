#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026900.
 * sub_08026900 @ 0x08026900
 */

void SetFreeForAllTeams(void)
{
    gPlaySt.unk42[1] = 0;
    gPlaySt.unk42[2] = 1;
    gPlaySt.unk42[3] = 2;
    gPlaySt.unk42[4] = 3;
}
asm(".global sub_08026900\n.thumb_set sub_08026900, SetFreeForAllTeams\n");
