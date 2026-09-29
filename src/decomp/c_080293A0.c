#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080293A0.
 * sub_080293A0 @ 0x080293A0
 */

#include "proc.h"

/* THE PARAMETER IS INVISIBLE at the call: r0 is never written in this function,
 * so whatever arrives in it is what reaches StartRangeSpread. That callee's own
 * body settles it -- it moves r0 into r1 and hands it to Proc_StartBlocking as
 * the parent -- so this takes a ProcPtr and forwards it.
 *
 * The cursor copy is the `pos` view of the union, not `spos`: both halves are
 * plain `ldrh`/`strh` with no sign extension anywhere. */
void TargetPickCursor_Init(ProcPtr parent)
{
    gUnknown_03003F40 = 0;
    gUnknown_030033E4.unk00 = gUnknown_03003100.pos.unk00;
    gUnknown_030033E4.unk02 = gUnknown_03003100.pos.unk02;
    StartRangeSpread(parent);
}
asm(".global sub_080293A0\n.thumb_set sub_080293A0, TargetPickCursor_Init\n");
