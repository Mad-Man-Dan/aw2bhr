#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801153C.
 * sub_0801153C @ 0x0801153C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Family F062 (data/families.json): `push {r4,lr}; adds r4,r0,#0; bl A;
 * adds r0,r4,#0; bl B; pop {r4}; pop {r0}; bx r0` -- 20 bytes, three members.
 * Both `adds` immediates are absent from `varies`, so only the two callees
 * differ.
 *
 * This is NOT the wave-14 F032 shape: there is no `lsls r0,#0x18; cmp r0,#0;
 * bne`, so the second call is unconditional and the first callee's result is
 * discarded. Whether the FIRST call receives the incoming r0 is decided
 * callee-side and the three members do not agree on it -- see each below.
 * `pop {r4}; pop {r0}` is still the void epilogue.
 */


/* The `bl EndFadeScreenLines` with r0 still holding the incoming proc looks exactly
 * like an argument pass and is not one: EndFadeScreenLines is
 * `void EndFadeScreenLines(void)` in its promoted definition src/decomp/c_08011218.c
 * (`Proc_EndEach(ProcScr_FadeScreenLines)`), so it takes nothing. The
 * `adds r4, r0, #0` is "save it because the call clobbers r0", exactly the
 * wave-14 F032 trap one shape up.
 */

void Wipe_End(ProcPtr proc)
{
    EndFadeScreenLines();
    Proc_Break(proc);
}

asm(".global sub_0801153C\n.thumb_set sub_0801153C, Wipe_End\n");
