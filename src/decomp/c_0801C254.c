#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C254.
 * sub_0801C254 @ 0x0801C254
 */

/* ONE `if` with `||`, not two nested ones: both arms reach the SAME
 * `movs r0, #0`, and that block sits in the MIDDLE of the function with an
 * unconditional `b` to the epilogue -- the short-circuit layout. Two nested
 * `if`s would have put the early-out past the body.
 *
 * The declaration carries THREE parameters while this body touches only r0.
 * That is not a mistake to fix: it was derived in wave 29 from VolcanoRock_WaitImpact,
 * which materialises r1 and r2 before the `bl` and is already promoted. An
 * argument a body never reads costs no instruction, so the body is the weaker
 * evidence here; dropping the two would break the split build, which a
 * per-function try_match cannot see.
 *
 * WAVE 36 (W36-H): the two extra arguments are FORWARDED, not dead. AP_Display
 * was matched this wave and its body reads r1 and r2, so the call below now
 * passes all three; it is byte-identical either way, because r1 and r2 already
 * hold them. */
u8 AP_Update(struct Unk0801C210 *a1, int a2, int a3)
{
    if (a1 == NULL || a1->unk00 == NULL)
        return 0;

    AP_Display(a1, a2, a3);
    return AP_ExecFrame(a1);
}
asm(".global sub_0801C254\n.thumb_set sub_0801C254, AP_Update\n");
