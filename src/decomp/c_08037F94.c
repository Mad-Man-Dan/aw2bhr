#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037F94.
 * sub_08037F94 @ 0x08037F94, sub_08037FB4 @ 0x08037FB4
 */

#include "proc.h"

/* The FIRST parameter is dead: the body saves r1 into r4 and never reads r0
 * at all. It is kept because both of this pair's siblings in the block
 * (sub_08037FB4 below it, and the StartLockingFadeToBlack family) are `(int, ProcPtr)`
 * proc starters, and dropping it would leave callers passing an argument the
 * prototype does not declare. */
void sub_08037F94(int a, ProcPtr parent)
{
    sub_0801237C();
    sub_08012358();
    Proc_StartBlocking(gUnknown_08615CB0, parent);
}

void sub_08037FB4(ProcPtr parent)
{
    sub_0801237C();
    sub_08012358();
    sub_08049F08(1, parent);
}
