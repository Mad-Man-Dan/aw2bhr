#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CE84.
 * sub_0803CE84 @ 0x0803CE84
 */

/* `int` parameter with a `(u8)` cast at the use, not a `u8` parameter: the ROM
 * copies first and narrows second (`adds r2, r0, #0; lsls #0x18; lsrs #0x18`),
 * which is copy-then-narrow. PROMOTE_MODE narrowing of a sub-word parameter
 * works on r0 in place, before any copy.
 *
 * The `u8` return is forced by the trailing shift pair together with
 * DrawDesignRoomSlotPreview's declared `int` return: if both were `int` there would be no
 * pair at all, and the pair is here. */
u8 DrawDesignRoomSlotPreviewAtOrigin(int a)
{
    return DrawDesignRoomSlotPreview(0, 0, (u8)a);
}
asm(".global sub_0803CE84\n.thumb_set sub_0803CE84, DrawDesignRoomSlotPreviewAtOrigin\n");
