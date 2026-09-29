#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D770.
 * sub_0803D770 @ 0x0803D770
 */

#include "proc.h"

/* The existence predicate half of the start/exists pair: sub_0803D75C is the
 * starter for the same script. `cmp; beq; movs #1` with no second constant is
 * the `return c;` row of the two-valued table in docs/agbcc-codegen.md -- the
 * `if`/`return` spelling would materialise the zero and cost four bytes.
 */
int IsSaveScreenRunning(void)
{
    return Proc_Find(gUnknown_0849F330) != 0;
}
asm(".global sub_0803D770\n.thumb_set sub_0803D770, IsSaveScreenRunning\n");
