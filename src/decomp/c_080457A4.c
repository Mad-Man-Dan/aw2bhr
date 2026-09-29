#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080457A4.
 * sub_080457A4 @ 0x080457A4, sub_080457BC @ 0x080457BC, HasNoDeathRay @ 0x080457D0, HasNoLaser @ 0x080457E8, HasNoBlackCannon @ 0x08045800, HasNoMinicannon @ 0x08045818, HasNoPipeSeams @ 0x08045830
 */

#include "proc.h"

/* `!= 0` rather than the if/else pair: the original reuses the r0 that `cmp`
 * just tested as the zero result, so there is no `movs r0, #0` anywhere. The
 * two if/else spellings both emit one. */
int sub_080457A4(void)
{
    return Proc_Find(gUnknown_084A0A3C) != 0;
}

int IsTwoOptionChoiceFirst(void)
{
    if (gUnknown_03002EE4 != 0)
        return 0;

    return 1;
}
asm(".global sub_080457BC\n.thumb_set sub_080457BC, IsTwoOptionChoiceFirst\n");

int HasNoDeathRay(void)
{
    if (HasLivingInventionOfType(5) == 0)
        return 1;

    return 0;
}

asm(".global sub_080457D0\n.thumb_set sub_080457D0, HasNoDeathRay\n");

int HasNoLaser(void)
{
    if (HasLivingInventionOfType(1) == 0)
        return 1;

    return 0;
}

asm(".global sub_080457E8\n.thumb_set sub_080457E8, HasNoLaser\n");

int HasNoBlackCannon(void)
{
    if (HasLivingInventionOfType(3) == 0)
        return 1;

    return 0;
}

asm(".global sub_08045800\n.thumb_set sub_08045800, HasNoBlackCannon\n");

int HasNoMinicannon(void)
{
    if (HasLivingInventionOfType(4) == 0)
        return 1;

    return 0;
}

asm(".global sub_08045818\n.thumb_set sub_08045818, HasNoMinicannon\n");

int HasNoPipeSeams(void)
{
    if (AnyPipeSeamHpSet() == 0)
        return 1;

    return 0;
}

asm(".global sub_08045830\n.thumb_set sub_08045830, HasNoPipeSeams\n");
