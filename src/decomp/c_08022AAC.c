#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022AAC.
 * sub_08022AAC @ 0x08022AAC
 */

/* Wave 31, W31-A RETYPED the parameters u16 -> s16, and re-verified this
 * function byte-for-byte afterwards. Nothing in this body can tell the two
 * apart: agbcc's PROMOTE_MODE narrows a parameter of either signedness with
 * `lsls #0x10; lsrs #0x10`, and `strh` and `<< 4` are both sign-blind. The
 * evidence is on the caller side, which did not exist in C until wave 31 --
 * MapCursorState_OpenUnitMenu and MapCursorState_Ambushed pass gUnknown_03003100.spos.unk00/.unk02
 * through with `ldrsh` and no zero-extension, which a u16 parameter would have
 * forced. See the declaration comment in include/unknown-functions.h. */
void SetMapCursorPosition(s16 x, s16 y)
{
    gUnknown_030033E4.unk00 = x;
    gUnknown_030033E4.unk02 = y;
    gUnknown_030033E0.unk00 = x << 4;
    gUnknown_030033E0.unk02 = y << 4;
}
asm(".global sub_08022AAC\n.thumb_set sub_08022AAC, SetMapCursorPosition\n");
