#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804EDAC.
 * sub_0804EDAC @ 0x0804EDAC
 */

/* One frame of a 0x90-step vertical wobble: the slot's own unk28 is the phase,
 * and gUnknown_085523E0 the offset table it indexes.
 *
 * THE SUM IS AN `int` TEMPORARY, and that is what makes the table read `ldrsh`.
 * Written as `entries[b].y += gUnknown_085523E0[phase]` the whole addition
 * happens in HImode -- only the low halfword survives the `strh` -- so agbcc
 * drops the sign extension and emits `ldrh`, losing the one instruction that
 * proves the table is signed. Computing the sum in a word and storing it is
 * byte-exact and keeps the `movs r3, #0; ldrsh` register-offset form. */
void StepFigureBob(u16 a, u16 b, s16 c)
{
    int v;

    gUnknown_03001470[c].unk28++;
    if (gUnknown_03001470[c].unk28 == 0x90)
        gUnknown_03001470[c].unk28 = 0;

    v = gUnknown_085523E0[gUnknown_03001470[c].unk28]
        + gUnknown_02029A10[a].entries[b].y;
    gUnknown_02029A10[a].entries[b].y = v;
}
asm(".global sub_0804EDAC\n.thumb_set sub_0804EDAC, StepFigureBob\n");
