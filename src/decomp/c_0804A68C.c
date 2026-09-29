#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A68C.
 * sub_0804A68C @ 0x0804A68C
 */

void NameEntry_DeleteChar(void)
{
    gUnknown_030044E0->unk2c[gUnknown_030044E0->unk5d] = 0;
}
asm(".global sub_0804A68C\n.thumb_set sub_0804A68C, NameEntry_DeleteChar\n");
