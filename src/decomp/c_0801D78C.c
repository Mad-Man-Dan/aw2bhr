#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D78C.
 * sub_0801D78C @ 0x0801D78C
 */

int StartSpriteScriptInRange(int a1, int a2, int a3, int a4, int a5, int a6)
{
    int i;

    for (i = a5; i <= a6; i++)
        if (gUnknown_0200E438[i].unk08 == 0)
            return InitSpriteScript(i, a1, a2, a3, a4);

    return -1;
}
asm(".global sub_0801D78C\n.thumb_set sub_0801D78C, StartSpriteScriptInRange\n");
