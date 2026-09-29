#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022AD0.
 * sub_08022AD0 @ 0x08022AD0
 */

/* Both parameters are `s16` and not the `u16` this was promoted with. The body
 * is two plain `strh`s and is byte-identical either way, so the evidence is
 * entirely on the caller side: Menu_PlaceCursorSprite (wave 26) passes an `ldrsh` member
 * with no conversion and a computed expression narrowed `lsls #0x10; asrs
 * #0x10`, where the u16 spelling probes as `ldrh` plus a zero-extending `lsrs`.
 * Re-verified byte-exact with trymatch after the change. */
void SetMapCursorDisplayPosition(s16 x, s16 y)
{
    gUnknown_030033E0.unk00 = x;
    gUnknown_030033E0.unk02 = y;
}
asm(".global sub_08022AD0\n.thumb_set sub_08022AD0, SetMapCursorDisplayPosition\n");
