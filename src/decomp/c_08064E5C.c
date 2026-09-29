#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064E5C.
 * sub_08064E5C @ 0x08064E5C
 */

/* `0xC00 | obj->unk44` and not the other order: the ROM materialises the
 * constant into the destination register first (`adds r2, r7, #0; orrs r2, r3`),
 * which is operand 0. */
void ArmyColumn_Draw(struct Unk08580934_Obj *obj)
{
    DrawArmySlotControllerIcon(obj->unk28 + 0x20, obj->unk2a, obj->unk1c);
    DrawArmySlotColorIcon(obj->unk28 + 0x20, obj->unk2a + 0x10, obj->unk1c);
    sub_08043FD8((obj->unk28 + 0x18) & 0x1FF, (obj->unk2a + 0x30) & 0xFF,
                 0xC00 | obj->unk44, 2);
    DrawOamObject(0xBC, (obj->unk28 - 8) & 0x1FF, (obj->unk2a - 8) & 0xFF, 0, 1);
    obj->unk49 = obj->unk48;
}
asm(".global sub_08064E5C\n.thumb_set sub_08064E5C, ArmyColumn_Draw\n");
