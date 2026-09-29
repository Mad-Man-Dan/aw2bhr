#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DED8.
 * sub_0801DED8 @ 0x0801DED8, sub_0801DF20 @ 0x0801DF20
 */

void DrawSimpleSpriteScripts(void)
{
    int i;

    for (i = 0; i < gUnknown_03003034; i++)
    {
        if (gUnknown_0200E438[i].unk08)
        {
            RunSimpleSpriteScript(i, 0);
            UpdateSpriteScriptAffine(i);
        }
    }
}
asm(".global sub_0801DED8\n.thumb_set sub_0801DED8, DrawSimpleSpriteScripts\n");

void TickSimpleSpriteScripts(void)
{
    int i;

    for (i = 0; i < gUnknown_03003034; i++)
        if (gUnknown_0200E438[i].unk08)
            RunSimpleSpriteScript(i, 1);
}
asm(".global sub_0801DF20\n.thumb_set sub_0801DF20, TickSimpleSpriteScripts\n");
