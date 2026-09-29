#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804026C.
 * sub_0804026C @ 0x0804026C, sub_08040290 @ 0x08040290
 */

#include "proc.h"

/* Asks GetInventionTargetCell for the entry's tile position into a stack pair, then
 * starts the 0x0849FADC proc through sub_0803FEDC with that position and its
 * own parent argument. GetInventionTargetCell returns bool8 and the result is dropped --
 * no narrowing follows the `bl`. The twin sub_08040290 differs only in calling
 * sub_0803FF04. */
void sub_0804026C(struct Unk02028360 *ent, ProcPtr parent)
{
    struct Unk02028360Pos pos;

    GetInventionTargetCell(ent, &pos);
    sub_0803FEDC(pos.unk00, pos.unk02, parent);
}

/* The twin of sub_0804026C, differing only in which of the two 0x0849FADC
 * starters it calls (sub_0803FF04 rather than sub_0803FEDC). */
void sub_08040290(struct Unk02028360 *ent, ProcPtr parent)
{
    struct Unk02028360Pos pos;

    GetInventionTargetCell(ent, &pos);
    sub_0803FF04(pos.unk00, pos.unk02, parent);
}
