#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DC04.
 * sub_0801DC04 @ 0x0801DC04
 */

/* Find-first-free over the gUnknown_0200E438 slot array, then hand the slot to
 * InitSpriteScriptFromTable to initialise. The `(s16)` on the result is real: the return
 * type is `int` (see unknown-functions.h -- the ROM narrows here at #0x10,
 * which an s8 return could not produce), so nothing else would emit it.
 */
int StartSpriteScriptFromTable(void *a1, s16 a2, s16 a3)
{
    s16 i;

    for (i = 0; i <= 0x1E; i++)
        if (gUnknown_0200E438[i].unk08 == 0)
            return (s16)InitSpriteScriptFromTable(i, a1, a2, a3);
    return -1;
}
asm(".global sub_0801DC04\n.thumb_set sub_0801DC04, StartSpriteScriptFromTable\n");
