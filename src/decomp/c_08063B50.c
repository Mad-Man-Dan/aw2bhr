#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063B50.
 * sub_08063B50 @ 0x08063B50
 */

void sub_08063B50(struct Unk8063BE0 *p)
{
    if (p->unk42 != 0)
    {
        p->unk42--;
        return;
    }

    if (p->unk44 > 0)
    {
        p->unk44--;
        p->unk2c = p->unk2c * 3 / 4;
        if (p->unk44 == 10)
            gUnknown_0202F0E8[2]--;
    }

    DrawZoomedSpriteFromCentre(p->unk24, p->unk28, p->unk2c, p->unk48, p->unk1c);
}
