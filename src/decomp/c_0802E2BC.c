#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E2BC.
 * sub_0802E2BC @ 0x0802E2BC
 */

/* Family F077: run a routine, then park a state-machine id in a u16 global.
 * `gUnknown_03003334` is spelled directly, which is what the ROM does -- the
 * `-fforce-addr` .rodata pool word at 0x08090D88 that unknown-globals.h warns
 * about applies to RunMapStateMachine only, where the address is live across a
 * switch; here it dies inside one statement. */

void StartUnitsTranslucentPeek(void)
{
    SetMapLayersUnitsTranslucent();
    gUnknown_03003334 = 8;
}
asm(".global sub_0802E2BC\n.thumb_set sub_0802E2BC, StartUnitsTranslucentPeek\n");
