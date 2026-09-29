#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019818.
 * sub_08019818 @ 0x08019818
 */

/* `b` is reassigned rather than copied into a second local: the recomputed
 * value comes back through the same r1 the `cmp r1, #0x17` tested and is
 * re-truncated `lsls #0x18; lsrs #0x18`, which is the u8 assignment.
 *
 * `a` is `u16` and not `s16`: PROMOTE_MODE zero-extends it at entry either way,
 * but it is then stored as a whole word at +0x14 with no second extension, and
 * an `s16` would have to be sign-extended there. */
void StartCoSpeechScript(u16 a, u8 b, u8 c)
{
    if (b <= 0x17)
        b = b + c * 24;

    gUnknown_03002F08.unk02 = b;
    StartEventScript(gUnknown_0848A3EC)->unk14 = a;
}
asm(".global sub_08019818\n.thumb_set sub_08019818, StartCoSpeechScript\n");
