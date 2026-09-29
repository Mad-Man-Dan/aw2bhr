#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C3EC.
 * sub_0801C3EC @ 0x0801C3EC
 */

#include "hardware.h"

/* MATCHED, wave 42 (W42-E). Byte-for-byte identical, relocs match.
 *
 * Builds the four OAM affine parameters for each entry of the sprite handle's
 * affine list and hands them to SetObjAffine. unk14 points at a u16 header
 * whose first halfword is the entry count in its low 15 bits; the entries
 * start one halfword later and are three halfwords each (angle, then the two
 * scale divisors), hence `p += 3`.
 *
 * TWO THINGS DECIDED THE MATCH, both about where a narrowing lands.
 *
 * 1. The Div results are NOT s16 locals. agbcc stores a narrow local
 *    ZERO-extended (PROMOTE_MODE) and sign-extends it again at each use, so an
 *    s16 local emits `lsl #16; lsr #16` after the call AND a second
 *    `lsl #16; asr #16` at the call site -- eight instructions the ROM has
 *    not got. The ROM has exactly one `lsl #16; asr #16` per value, right
 *    after its `bl`, which is an s32 narrowed once by a cast.
 *
 * 2. The four Div calls are INLINE ARGUMENTS, not assignments to locals, and
 *    this is what fixed the last 13 bytes (94.4% -> match). With locals, the
 *    fourth result's sign-extension sinks into the argument setup and the
 *    index is evaluated first, into r1 and then copied to r0. The ROM does the
 *    opposite: it narrows the fourth result into r1 and only then computes
 *    `a1->unk21 + i` straight into r0. That is gcc's expand_call PRECOMPUTING
 *    EVERY ARGUMENT THAT CONTAINS A CALL before the ones that do not -- the
 *    first three results take callee-saved r6/r5/r4 because they must survive
 *    the later calls, the fourth takes scratch r1 because it need not, and the
 *    call-free index argument is evaluated last, directly into its hard
 *    register. Assigning to locals never reaches that path.
 *
 *    Worth carrying forward as a read-out rule: when a call's arguments land
 *    in callee-saved registers in source order with the LAST one in a scratch,
 *    and a simple argument is computed after all of them, the calls were
 *    written inline in the argument list.
 *
 * The gSinLut base and the 0xff mask in the preheader are LICM hoists sitting
 * after the loop guard -- not authored, correctly. */
void AP_QueueObjRotScale(struct Unk0801C210 *a1)
{
    u16 *p;
    int i;
    int n;

    if (a1 != NULL && a1->unk00 != NULL && a1->unk14 != NULL)
    {
        n = a1->unk14[0] & 0x7fff;
        p = a1->unk14 + 1;

        for (i = 0; i < n; i++)
        {
            SetObjAffine(a1->unk21 + i,
                         (s16)Div(COS_Q12(p[0]) << 4, p[1]),
                         (s16)Div(-SIN_Q12(p[0]) << 4, p[2]),
                         (s16)Div(SIN_Q12(p[0]) << 4, p[1]),
                         (s16)Div(COS_Q12(p[0]) << 4, p[2]));

            p += 3;
        }
    }
}
asm(".global sub_0801C3EC\n.thumb_set sub_0801C3EC, AP_QueueObjRotScale\n");
