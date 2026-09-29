#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007B74.
 * sub_08007B74 @ 0x08007B74
 */

/* MATCHED. Byte-for-byte the same function as sub_08005F1C -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void sub_08007B74(void)
{
    if (gActiveMap->spriteId != -1)
    {
        EndSlotScriptAt(gActiveMap->spriteId);
        gActiveMap->spriteId = 0xFF;
    }
}
