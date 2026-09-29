#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F084.
 * sub_0801F084 @ 0x0801F084
 */

/* Family F070's shape one more time (see src/decomp/c_0801F050.c): an if/else
 * on gUnknown_03001FE0, the "is there a deferred execution context" word. */
void FlushSpritesForMode(void)
{
    if (gUnknown_03001FE0) {
        PushSpriteLayerObjects(0);
        PushSpriteLayerObjects(5);
        ClearSprites();
    } else {
        FlushSpriteRequests();
    }
}
asm(".global sub_0801F084\n.thumb_set sub_0801F084, FlushSpritesForMode\n");
