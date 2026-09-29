#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C1D0.
 * sub_0802C1D0 @ 0x0802C1D0
 */

/* One nested call. The `lsls #0x18; asrs #8; lsrs #0x10` triple between the two
 * `bl`s is a single conversion, not two: GetSuspendIdForGameMode returns s8 and
 * WriteSuspendSave's first parameter is u16, so the value is sign-extended from 8
 * bits and then truncated to 16, which agbcc fuses into one left shift and two
 * rights. Both widths are independently confirmed on the callee side --
 * GetSuspendIdForGameMode's declaration in unknown-functions.h and WriteSuspendSave's own
 * prologue (`lsls r0,#0x10; lsrs r4,#0x10` then `lsls r1,#0x18; lsrs r7,#0x18`).
 * An `int` parameter would have left a bare `lsls #0x18; asrs #0x18` pair and an
 * s8 one nothing at all.
 *
 * The narrowing is the only thing between the calls, so this is genuine nesting
 * and not two statements. `pop {r0}; bx r0`, so void.
 */

void WriteSuspendSaveForCurrentMode(void)
{
    WriteSuspendSave(GetSuspendIdForGameMode(gPlaySt.gameMode), 0);
}
asm(".global sub_0802C1D0\n.thumb_set sub_0802C1D0, WriteSuspendSaveForCurrentMode\n");
