#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804EA24.
 * sub_0804EA24 @ 0x0804EA24
 */

/* Two calls with the same three arguments. Sequential bls carry no nesting
 * signal, but here there cannot be any: StreamBodyPose is void, and all three
 * values are reloaded from callee-saved registers before the second call. The
 * six lsls/lsrs pairs are the u16 parameters' entry narrowing, which is also
 * what forces them into r4/r5/r6 in the first place. */
void ResetFigurePose_BodyCrew(u16 a, u16 b, u16 c)
{
    StreamBodyPose(a, b, c);
    StreamCrewPose(a, b, c);
}
asm(".global sub_0804EA24\n.thumb_set sub_0804EA24, ResetFigurePose_BodyCrew\n");
