#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043D84.
 * GetCoMusic @ 0x08043D84
 */

u16 GetCoMusic(u8 a1)
{
    return gUnknown_085D3DD0[gPlayers[a1].co].music;
}

asm(".global sub_08043D84\n.thumb_set sub_08043D84, GetCoMusic\n");
