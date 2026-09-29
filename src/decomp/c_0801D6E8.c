#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D6E8.
 * sub_0801D6E8 @ 0x0801D6E8
 */

int InitSpriteScript(int a1, int a2, int a3, int a4, int a5)
{
    struct Unk0200E438 *e;

    if (a5 > 0x1F)
    {
        a5 = AllocObjAffineRecord();
        if (a5 == -1)
            return a5;
    }

    e = &gUnknown_0200E438[a1];
    e->unk02 = 0;
    e->unk00 = 0;
    e->unk08 = a2;
    e->unk04 = a2;
    e->unk10 = 0;
    e->unk0c = 0;
    e->unk18 = 0;
    e->unk14 = 0;
    e->unk1c = 0;
    e->unk20 = a3;
    e->unk1e = a4;
    e->unk28 = a5;
    e->unk26 = 0;
    e->unk2c = 0;
    e->unk24 = 0;
    e->unk38 = 0xFFFF;
    e->unk3a = -1;
    e->unk3c = 0x100;
    e->unk3e = 0x100;
    e->unk40 = 0;
    e->unk44 = 0;
    *(long long *)&e->unk30 = 0;
    if (a1 >= gUnknown_03003034)
        gUnknown_03003034 = a1 + 1;
    return a1;
}
asm(".global sub_0801D6E8\n.thumb_set sub_0801D6E8, InitSpriteScript\n");
