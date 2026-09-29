#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C4B8.
 * sub_0802C4B8 @ 0x0802C4B8, sub_0802C4D4 @ 0x0802C4D4
 */

#include "proc.h"

/* Family F075: reset some state, then start a proc under the caller's parent.
 * The incoming r0 is saved into r4 across the first call and re-emerges as
 * Proc_StartBlocking's second argument, so the parameter is the parent -- and
 * the first call takes NOTHING. That is read off the callee, not off the call
 * site: BuildAttackTargetList's first instruction is `bl ClearAttackTargetList`, and
 * ClearAttackTargetList only stores 0 to gUnknown_030040A8, so no argument register is
 * consumed anywhere in the chain. A pass-through would be byte-identical here,
 * which is exactly why the readout has to come from the callee side.
 *
 * gUnknown_0849A02C is newly declared `const struct ProcCmd []`: DrawCursorDamagePreview
 * and DrawArmyCaptureCounters hand it straight to Proc_Find, and it is the 0x20-byte slot
 * immediately after gUnknown_0849A00C, which is already declared that way. */

void StartTargetPickList(ProcPtr parent)
{
    BuildAttackTargetList();
    Proc_StartBlocking(gUnknown_0849A02C, parent);
}
asm(".global sub_0802C4B8\n.thumb_set sub_0802C4B8, StartTargetPickList\n");

/* Family F075, second member -- see StartTargetPickList. Same teardown call, the next
 * proc script along. */

void StartTargetPickCursor(ProcPtr parent)
{
    BuildAttackTargetList();
    Proc_StartBlocking(gUnknown_0849A04C, parent);
}
asm(".global sub_0802C4D4\n.thumb_set sub_0802C4D4, StartTargetPickCursor\n");
