#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E13C.
 * sub_0801E13C @ 0x0801E13C
 */

/* MATCHED (wave 40, W40-J), byte-for-byte, first attempt.
 *
 * Scans the 32 slot-in-use flags for the first free one, claims it, clears the
 * record's two interpolation counters and returns the index, or -1.
 *
 * Two things in the ROM look authored and are not. The dead `ldrb` in front of
 * the `strb` is gUnknown_030024F0's volatile-element tell -- the same one
 * FreeObjAffineRecord and InitOamRequestsAndObjAffine carry, already recorded on the declaration in
 * include/unknown-globals.h. And the two `strh` stores use the register holding
 * the LOADED BYTE rather than a fresh zero, because cse knows the flag compared
 * equal to 0 on this path; writing `= 0` is what produces that.
 *
 * gUnknown_0200F720 is referenced TWICE here and gets a plain text pool word.
 * Its neighbour StepObjAffineTweens references the same object ONCE and gets a
 * -fforce-addr .rodata address constant instead. The two make a controlled pair
 * for that threshold. */
int AllocObjAffineRecord(void)
{
    int i;

    for (i = 0; i <= 0x1f; i++)
    {
        if (gUnknown_030024F0[i] == 0)
        {
            gUnknown_030024F0[i] = 1;
            gUnknown_0200F720[i].unk06 = 0;
            gUnknown_0200F720[i].unk0c = 0;
            return i;
        }
    }
    return -1;
}
asm(".global sub_0801E13C\n.thumb_set sub_0801E13C, AllocObjAffineRecord\n");
