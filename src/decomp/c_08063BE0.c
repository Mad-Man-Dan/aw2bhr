#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063BE0.
 * sub_08063BE0 @ 0x08063BE0
 */

#include "hardware.h"

/* struct Unk8063BE0 moved to include/unknown-globals.h in wave 30 (W30-C),
 * layout unchanged: sub_08063BBC seeds unk44 and tail-calls this function on
 * the same pointer, so the type has to be visible in both units. */

void sub_08063BE0(struct Unk8063BE0 *p)
{
    if (gUnknown_0202F0E8[2] != 0)
    {
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
    else
    {
        p->unk44--;

        SetObjAffine(0,
                     Div(COS_Q12(0) * 16, 0x100),
                     Div(-SIN_Q12(0) * 16,
                         p->unk44 * 32 != 0 ? p->unk44 * 32 : 2),
                     Div(SIN_Q12(0) * 16, 0x100),
                     Div(COS_Q12(0) * 16,
                         p->unk44 * 32 != 0 ? p->unk44 * 32 : 2));

        PutOamHi(p->unk24 + 0x78, p->unk28 + 0x58 - p->unk44,
                     gUnknown_085806F2, p->unk1c * 4);

        if (p->unk44 <= 1)
            ClearSlotScriptCallback(gUnknown_03001FBC);
    }
}
