#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029D3C.
 * sub_08029D3C @ 0x08029D3C
 */

void SupplyCommand_Init(struct Unk03001470 *p)
{
    u16 i;

    gUnknown_03003F40 = 0;

    for (i = 0; i < 4; i++)
    {
        gUnknown_03003F40 |= (s16)gUnknown_0849A0D8[i * 3]
            * IsResupplyableAllyAt(
                gUnknown_03003100.pos.unk00 + gUnknown_0849A0D8[i * 3 + 1],
                gUnknown_03003100.pos.unk02 + gUnknown_0849A0D8[i * 3 + 2]);
    }

    p->unk1e = 0;
}
asm(".global sub_08029D3C\n.thumb_set sub_08029D3C, SupplyCommand_Init\n");
