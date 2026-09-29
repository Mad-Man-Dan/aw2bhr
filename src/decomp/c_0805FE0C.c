#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FE0C.
 * sub_0805FE0C @ 0x0805FE0C
 */

void AiExecutorBegin(void)
{
    SetRandomSeed(gUnknown_030046C0.unk08);

    if (gUnknown_030046C0.unk00 == 1
     || gUnknown_030046C0.unk00 == 0xe
     || gUnknown_030046C0.unk00 == 0xd
     || gUnknown_030046C0.unk00 == 0xf
     || gUnknown_030046C0.unk00 == 0x10
     || gUnknown_030046C0.unk00 == 0x11
     || gUnknown_030046C0.unk00 == 0x12
     || gUnknown_030046C0.unk00 == 0x13)
    {
        if (gUnknown_030046C0.unk00 == 0x11)
        {
            gUnknown_03003F38 = gUnknown_030046C0.unk01;
            gUnknown_030040D8 = (struct Unk030040D8 *)&gUnits[gUnknown_03003F38];
            gUnknown_030040D8->unk06 = gUnknown_030046C0.unk12;
        }

        if (gUnknown_030046C0.unk00 == 0x12)
        {
            gUnknown_03003F38 = gUnknown_030046C0.unk01;
            gUnknown_030040D8 = (struct Unk030040D8 *)&gUnits[gUnknown_03003F38];
            ScrollCameraToKeepCellInView(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03);
        }

        gUnknown_030045D4 = 2;
        return;
    }

    gUnknown_03003F38 = gUnknown_030046C0.unk01;
    gUnknown_030040D8 = (struct Unk030040D8 *)&gUnits[gUnknown_03003F38];

    gUnknown_03003F24.pos.unk00 = gUnknown_030046C0.filler_04[0];
    gUnknown_03003F24.pos.unk02 = gUnknown_030046C0.filler_04[1];
    gUnknown_03003100.pos.unk00 = gUnknown_030046C0.unk02;
    gUnknown_03003100.pos.unk02 = gUnknown_030046C0.unk03;

    gUnknown_030040D8->unk01 &= ~4;

    if (gUnknown_030032D8 == 0x13)
    {
        gUnknown_03004074 = 0;
        gUnknown_030040D8->unk06 = gUnknown_030046C0.unk12;
        gUnknown_030045D4 = 1;
    }

    if (gPlaySt.savingEnabled == 0 || gUnknown_03003F60 != 4)
        ScrollCameraToKeepCellInView(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03);

    gUnknown_030045D4 = 1;
}
asm(".global sub_0805FE0C\n.thumb_set sub_0805FE0C, AiExecutorBegin\n");
