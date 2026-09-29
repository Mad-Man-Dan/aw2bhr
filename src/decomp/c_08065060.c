#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065060.
 * sub_08065060 @ 0x08065060
 */

/* `o->unk3a += o->unk3c` and NOT `o->unk3a = o->unk3c + o->unk3a`: agbcc
 * evaluates the second operand first, so the compound form is what loads
 * +0x3c ahead of +0x3a, the way the ROM has it. The `lsls #0x10; asrs #0x10`
 * before the compare is the signed read of the s16 unk2a it just stored. */
void ArmyColumnExitUp_Loop(struct Unk08580934_Obj *o)
{
    o->unk3a += o->unk3c;
    o->unk2a += o->unk3a;

    if (o->unk2a < -0x28)
        ClearSlotScriptCallback(gUnknown_03001FBC);

    ArmyColumn_Draw(o);
}
asm(".global sub_08065060\n.thumb_set sub_08065060, ArmyColumnExitUp_Loop\n");
