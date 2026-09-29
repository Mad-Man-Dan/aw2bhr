#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080466A4.
 * sub_080466A4 @ 0x080466A4
 */

/* A 56-frame animation cursor with a hold counter beside it: wrap unk58 past
 * 0x37, then either advance it or burn one frame of unk59.
 *
 * BOTH bytes are read before the branch in the ROM, and that is what the two
 * locals buy. Written as `unk58++` / `unk59--` inside the arms instead, agbcc
 * loads unk59, branches, and only then re-derives unk58's address and value
 * inside the taken arm -- same length, different bytes.
 *
 * `bls` on the first test makes unk58 unsigned. */
void IntelStatus_StepPhase(void)
{
    u8 a;
    u8 b;

    if (gUnknown_084C1430->unk58 > 0x37)
        gUnknown_084C1430->unk58 = 0;

    a = gUnknown_084C1430->unk58;
    b = gUnknown_084C1430->unk59;

    if (b == 0)
        gUnknown_084C1430->unk58 = a + 1;
    else
        gUnknown_084C1430->unk59 = b - 1;
}
asm(".global sub_080466A4\n.thumb_set sub_080466A4, IntelStatus_StepPhase\n");
