#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080606BC.
 * sub_080606BC @ 0x080606BC
 */

/* Family F031. StartUnitDestroyed was promoted taking `s32`; this call site and
 * WriteBackBattleUnit both hand it a pointer, so it was retyped to `void *` (the two
 * objects are different types, so neither struct fits both). The change is
 * byte-neutral -- the value is only ever `str`-ed -- and c_0804018C.c was
 * re-verified with trymatch. */

void AiExecuteDestroyUnit(void)
{
    StartUnitDestroyed(gUnknown_030040D8);
}
asm(".global sub_080606BC\n.thumb_set sub_080606BC, AiExecuteDestroyUnit\n");
