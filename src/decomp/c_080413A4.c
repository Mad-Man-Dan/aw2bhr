#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080413A4.
 * sub_080413A4 @ 0x080413A4
 */

struct Unk03003338 *GetAttackTargetRecord(int index)
{
    return &gUnknown_03003338[index];
}
asm(".global sub_080413A4\n.thumb_set sub_080413A4, GetAttackTargetRecord\n");
