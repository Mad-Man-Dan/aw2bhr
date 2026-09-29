#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D3AC.
 * sub_0806D3AC @ 0x0806D3AC
 */

void RulesScreenArmyColumn_Draw(struct Unk08580934_Obj *obj)
{
    int y;

    DrawArmySlotControllerIcon(obj->unk28 + 0x20, obj->unk2a, obj->unk1c);
    DrawArmySlotColorIcon(obj->unk28 + 0x20, obj->unk2a + 0x10, obj->unk1c);

    sub_08043FD8((obj->unk28 + 0x18) & 0x1ff, (obj->unk2a + 0x30) & 0xff,
        0xc00 | obj->unk44, 2);

    DrawOamObject(0xbc, (obj->unk28 - 8) & 0x1ff, (obj->unk2a - 8) & 0xff, 0, 1);

    y = obj->unk2a;
    DrawOamObject(gUnknown_08580934->unk11[obj->unk1c] + 0xbd, obj->unk28 & 0x1ff,
        (y + 0x34) & 0xff, 0, 0);

    obj->unk49 = obj->unk48;
}
asm(".global sub_0806D3AC\n.thumb_set sub_0806D3AC, RulesScreenArmyColumn_Draw\n");
