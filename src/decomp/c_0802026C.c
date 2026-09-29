#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802026C.
 * sub_0802026C @ 0x0802026C
 */

/* Save-and-restore around one indirect call: raise gUnknown_03004480 to the
 * caller's value, dispatch, then put the global back to gUnknown_030033EC.
 *
 * `bl _call_via_r4` is ordinary agbcc output, not hand-written -- ARMv4T has no
 * `blx rN`, so every indirect THUMB call goes through a libgcc trampoline. The
 * register index is r4 because r0-r3 are all carrying arguments and the pointer
 * needs somewhere else to live.
 *
 * Five arguments to the callee and six to this function: the first is consumed
 * by the global, the rest are forwarded untouched, which is why none of them
 * costs an instruction. */
void GenerateMovementMapForArmy(int a1, int a2, int a3, int a4, int a5, int a6)
{
    gUnknown_03004480 = a1;
    gUnknown_030013EC(a2, a3, a4, a5, a6);
    gUnknown_03004480 = gUnknown_030033EC;
}
asm(".global sub_0802026C\n.thumb_set sub_0802026C, GenerateMovementMapForArmy\n");
