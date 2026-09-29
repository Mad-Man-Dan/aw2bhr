#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080546BC.
 * sub_080546BC @ 0x080546BC
 */

void SetBattleAnimFlagsForGame(void)
{
    gUnknown_03004504.bit0 = 0;
    gUnknown_03004504.bit1 = 1;
    gUnknown_03004504.bit2 = 0;
    gUnknown_03004504.bit3 = 0;
    gUnknown_03004504.bit4 = 1;
    gUnknown_03004504.bit5 = 1;
    gUnknown_03004504.bit6 = 1;
    gUnknown_03004504.unk02 = 0xffff;
}
asm(".global sub_080546BC\n.thumb_set sub_080546BC, SetBattleAnimFlagsForGame\n");
