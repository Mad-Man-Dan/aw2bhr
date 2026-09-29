#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019870.
 * sub_08019870 @ 0x08019870, sub_0801987C @ 0x0801987C, EnableCoPowers @ 0x08019888, DisableCoPowers @ 0x08019894, sub_080198A0 @ 0x080198A0
 */

void sub_08019870(void)
{
    gPlaySt.campaignRelated = 0;
}

void sub_0801987C(void)
{
    gPlaySt.campaignRelated = 1;
}

void EnableCoPowers(void)
{
    gPlaySt.coPowersEnabled = 1;
}

asm(".global sub_08019888\n.thumb_set sub_08019888, EnableCoPowers\n");

void DisableCoPowers(void)
{
    gPlaySt.coPowersEnabled = 0;
}

asm(".global sub_08019894\n.thumb_set sub_08019894, DisableCoPowers\n");

void SetCoScreenDrawHook(void (*func)(void))
{
    gUnknown_03002F20 = func;
}
asm(".global sub_080198A0\n.thumb_set sub_080198A0, SetCoScreenDrawHook\n");
