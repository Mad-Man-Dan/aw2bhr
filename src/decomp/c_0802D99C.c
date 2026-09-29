#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D99C.
 * sub_0802D99C @ 0x0802D99C
 */

/* The parameter is `s16`, and both halves of that are measured. The entry
 * `lsls #0x10; lsrs #0x10` is PROMOTE_MODE, which zero-extends regardless of
 * signedness and therefore says only "narrow"; the SIGN is in the second shift
 * pair, `lsls #0x10; asrs #0x10`, emitted at the use because DrawDeploymentList
 * takes an `int` and the value has to be widened before the `bl`.
 *
 * Sequential `bl`s with nothing between them carry no nesting signal, and
 * ClearDeploymentListArea reads no argument register, so these are two statements. */

void RedrawDeploymentList(s16 a1)
{
    ClearDeploymentListArea();
    DrawDeploymentList(a1);
}
asm(".global sub_0802D99C\n.thumb_set sub_0802D99C, RedrawDeploymentList\n");
