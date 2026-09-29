#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E698.
 * sub_0802E698 @ 0x0802E698, sub_0802E6C0 @ 0x0802E6C0, sub_0802E6F8 @ 0x0802E6F8
 */

/* Family F077 (see src/decomp/c_0802E2BC.c): do the work, then park a state id
 * in gUnknown_03003334.
 *
 * The two `ldrsh` reads are what forced SetMapCursorPosition's parameters from u16 to
 * s16 -- see the note on the declaration in include/unknown-functions.h. The
 * union's `spos` view is the signed one, and passing it to a u16 parameter
 * would have cost a zero-extending shift pair the ROM does not have. */
void MapCursorState_OpenUnitMenu(void)
{
    SetMapCursorPosition(gUnknown_03003100.spos.unk00, gUnknown_03003100.spos.unk02);
    sub_0802D558();
    gUnknown_03003334 = 4;
}
asm(".global sub_0802E698\n.thumb_set sub_0802E698, MapCursorState_OpenUnitMenu\n");

/* A guard whose two arms are wildly uneven, which is why the pool word for
 * gUnknown_030040D8 sits INSIDE the then-arm and gUnknown_03003334's after the
 * else label.
 *
 * GetUnitSelectionLock's result is re-narrowed with `lsls #0x18` before the test --
 * that is the tell for a u8 return, and its body is a single `ldrb`.
 *
 * unk02/unk03 are the byte halves of the cursor position (see the note on
 * struct Unk030040D8), and they feed MapCursor_OnPressA's two u16 parameters with no
 * conversion because a u8 already arrives zero-extended. */
void MapCursorState_UnitMenuOpen(void)
{
    if (GetUnitSelectionLock() == 0)
    {
        EndActiveMoveSlide();
        RebuildMapUnitLayers();
        MapCursor_OnPressA(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03);
    }
    else
    {
        gUnknown_03003334 = 0;
    }
}
asm(".global sub_0802E6C0\n.thumb_set sub_0802E6C0, MapCursorState_UnitMenuOpen\n");

/* MapCursorState_OpenUnitMenu's neighbour: same opening call on the same cursor position,
 * different middle, and state 0 instead of 4. Diffed against it rather than
 * derived from it -- two calls differ, not one constant. */
void MapCursorState_Ambushed(void)
{
    SetMapCursorPosition(gUnknown_03003100.spos.unk00, gUnknown_03003100.spos.unk02);
    LockUnitSelection();
    CommitUnitMove();
    gUnknown_03003334 = 0;
}
asm(".global sub_0802E6F8\n.thumb_set sub_0802E6F8, MapCursorState_Ambushed\n");
