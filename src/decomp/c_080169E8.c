#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080169E8.
 * sub_080169E8 @ 0x080169E8
 */

void EndAllSpriteScripts(void)
{
    u8 i;

    for (i = 0; i < 0x1e; i++) {
        if (gUnknown_0200E438[i].unk08 != 0)
            EndSpriteScript(i);
    }
}
asm(".global sub_080169E8\n.thumb_set sub_080169E8, EndAllSpriteScripts\n");
