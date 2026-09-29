#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BF2C.
 * sub_0801BF2C @ 0x0801BF2C
 */

void PushSpriteLayerObjects(int a1)
{
    struct SpriteEntry *node;

    for (node = &gUnknown_0200D510[a1]; node != NULL; node = node->next)
    {
        if (node->object != NULL)
        {
            if ((node->oam1 & 0x1000)
             && !((node->object[0] | (s16)node->oam0) & 0x100))
                sub_0801C090((s16)node->oam1, (s16)node->oam0, node->object,
                             node->oam2);
            else
                PutOamHi((s16)node->oam1, (s16)node->oam0, node->object,
                             node->oam2);
        }
    }
}
asm(".global sub_0801BF2C\n.thumb_set sub_0801BF2C, PushSpriteLayerObjects\n");
