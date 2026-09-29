#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DBF8.
 * sub_0802DBF8 @ 0x0802DBF8
 */

/* One `&&`, not two `if`s: the TRUE block sits between the second compare and
 * the literal pool, with the shared FALSE block past it, which is the shape
 * docs/agbcc-codegen.md records for `if (C1 && C2) return A; return B;`.
 * gUnknown_030033E0 is the {u16;u16} pair; both halves are tested at once
 * through a word view, hence the 0x000F000F pool word.
 */

bool8 IsMapCursorSettled(void)
{
    if ((gMap->unk10 & 0xF) == 0
     && (*(u32 *)&gUnknown_030033E0 & 0x000F000F) == 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802DBF8\n.thumb_set sub_0802DBF8, IsMapCursorSettled\n");
