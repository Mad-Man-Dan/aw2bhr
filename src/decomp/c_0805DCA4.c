#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DCA4.
 * sub_0805DCA4 @ 0x0805DCA4
 */

/* Dispatches the attacker's animation, but only while the 0xC0 group of
 * unk07[2] (offset 0x09) is clear.
 *
 * A SWITCH and not an if / else-if chain, and the two are distinguishable: the
 * ROM puts BOTH compares up front, ends the run with an unconditional `b` to
 * the join, and places the two call arms after the literal pool. An if /
 * else-if emits the first arm between the two compares instead. */
void AiDeliberatePickup(void)
{
    struct Unk030040D8 *p = gUnknown_030040D8;

    if ((p->unk07[2] & 0xC0) == 0)
    {
        switch (p->unk00)
        {
        case 7:
            AiDeliberateApcPickup();
            break;
        case 0x14:
            AiDeliberateTCopterPickup();
            break;
        }
    }
}
asm(".global sub_0805DCA4\n.thumb_set sub_0805DCA4, AiDeliberatePickup\n");
