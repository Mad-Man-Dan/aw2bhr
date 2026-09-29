#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080601C8.
 * sub_080601C8 @ 0x080601C8, sub_080601DC @ 0x080601DC
 */

void AiExecuteCoPower(void)
{
    PayForCoPower(gUnknown_030046C0.unk06, 1);
}
asm(".global sub_080601C8\n.thumb_set sub_080601C8, AiExecuteCoPower\n");

void AiExecuteSuperCoPower(void)
{
    PayForCoPower(gUnknown_030046C0.unk06, 2);
}
asm(".global sub_080601DC\n.thumb_set sub_080601DC, AiExecuteSuperCoPower\n");
