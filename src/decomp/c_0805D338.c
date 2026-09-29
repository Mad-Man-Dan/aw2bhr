#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805D338.
 * sub_0805D338 @ 0x0805D338
 */

/* Family F008, exemplar src/decomp/c_08010F94.c. The store is `strh`, and so
 * is the exemplar's -- the whole family is halfword stores and
 * gUnknown_03002B5C is declared u16. AiExecutorDispatchAction's shared epilogue writes 2
 * into the same global the same way. gUnknown_03004780 was retyped u16 -> s16
 * in wave 25 on RemoteTurn_ExecuteCommand's `ldrsh` read; a `movs`-sized constant stored
 * with `strh` is byte-identical either way, and this function was re-verified
 * after the change. */

void AiStartProductionPass(void)
{
    gUnknown_03004780 = 5;
}
asm(".global sub_0805D338\n.thumb_set sub_0805D338, AiStartProductionPass\n");
