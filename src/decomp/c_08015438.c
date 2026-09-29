#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015438.
 * sub_08015438 @ 0x08015438
 */


/*
 * StartSlotScriptWithSprite -- start a script in a free slot and give it a sprite.
 *
 * FindSlotScript finds a free gUnknown_03001470 slot (the first whose script
 * pointer is 0, or -1 when they are all taken) and StartSpriteScriptFromTable an OBJ, built
 * out of `c`, `d` and `e`. The two are then linked: the slot's .unk26 holds the
 * OBJ index and the OBJ's .unk38 holds the slot index. sub_08015224 installs
 * script blob `a` with mode `b`, and bit 1 of the mode word then marks the slot
 * as owning a sprite. Returns the slot index, or -1 if no slot or no OBJ was
 * free. sub_08015410 is a forwarder into this one that swaps arguments 3 and 4.
 *
 * Why the C looks odd: `i` is s8 and `j` is int, and the casts on `d`, `e` and
 * the StartSpriteScriptFromTable result are written out instead of being folded into
 * narrower types. Each cast is one sign-extension in the original; declaring
 * `j` as s8 would add a second one.
 */
s8 StartSlotScriptWithSprite(void *a, int b, void *c, void *d, int e)
{
    s8 i;
    int j;

    i = FindSlotScript(0);

    if (i != -1)
    {
        j = (s8)StartSpriteScriptFromTable(c, (s16)(int)d, (s16)e);

        if (j == -1)
            return j;

        gUnknown_03001470[i].unk26 = j;
        sub_08015224(a, i, (u8)b);
        gUnknown_03001470[i].unk12 |= 2;
        gUnknown_0200E438[j].unk38 = i;
    }

    return i;
}
asm(".global sub_08015438\n.thumb_set sub_08015438, StartSlotScriptWithSprite\n");
