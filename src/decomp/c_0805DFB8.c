#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DFB8.
 * sub_0805DFB8 @ 0x0805DFB8
 */

/* The counterattack twin of AiDeliberatePickup: the same switch on the same byte,
 * gated on the 0xC0 group being SET rather than clear, and dispatching to the
 * other pair of handlers. */
void AiDeliberateDeliver(void)
{
    struct Unk030040D8 *p = gUnknown_030040D8;

    if ((p->unk07[2] & 0xC0) != 0)
    {
        switch (p->unk00)
        {
        case 7:
            AiDeliberateApcDeliver();
            break;
        case 0x14:
            AiDeliberateTCopterDeliver();
            break;
        }
    }
}
asm(".global sub_0805DFB8\n.thumb_set sub_0805DFB8, AiDeliberateDeliver\n");
