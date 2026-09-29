#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D8E4.
 * sub_0801D8E4 @ 0x0801D8E4, sub_0801D924 @ 0x0801D924
 */

/* One of four whole-list sweeps over the gUnknown_0200E438 slot array. The
 * bound is gUnknown_03003034, the high-water mark InitSpriteScriptFromTable maintains; it is
 * re-read every iteration because the call inside the loop can move it, so it
 * must not be hoisted into a local. The `=gUnknown_0808F0A4` in the ROM's
 * literal pool is NOT a global -- it is agbcc's own -fforce-addr address
 * constant for gUnknown_03003034, which is why the read costs two `ldr`s.
 */
void TickSpriteScripts(void)
{
    int i;

    for (i = 0; i < gUnknown_03003034; i++)
        if (gUnknown_0200E438[i].unk08)
            sub_0801D390(i, 1);
}
asm(".global sub_0801D8E4\n.thumb_set sub_0801D8E4, TickSpriteScripts\n");

void DrawSpriteScripts(void)
{
    int i;

    for (i = 0; i < gUnknown_03003034; i++)
    {
        if (gUnknown_0200E438[i].unk08)
        {
            sub_0801D390(i, 0);
            UpdateSpriteScriptAffine(i);
        }
    }
}
asm(".global sub_0801D924\n.thumb_set sub_0801D924, DrawSpriteScripts\n");
