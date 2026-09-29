#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049E50.
 * sub_08049E50 @ 0x08049E50
 */

#include "hardware.h"
#include "proc.h"

/* Ends the sequence after 0xdc frames or on A. The flag is initialised to 0
 * BEFORE the guard -- `movs r2,#0` is the second instruction, ahead of the
 * gUnknown_02028E3C load -- and both arms set it to 1 rather than breaking
 * directly, which is why there is a `cmp r2,#1` at the end.
 *
 * The `lsls #0x10; lsrs #0x10` in front of `cmp r0,#0xdc` is the ++ result
 * re-narrowed: gUnknown_084C3240->unk2c is `u16`, cse keeps the incremented
 * value in the register and the compare is unsigned (`bls`). */
void DefeatFlow_WaitStep(ProcPtr proc)
{
    int flag;

    flag = 0;

    if (gUnknown_02028E3C != 0)
    {
        gUnknown_084C3240->unk2c++;

        if (gUnknown_084C3240->unk2c > 0xdc)
            flag = 1;

        if (gpKeySt->pressed & 1)
            flag = 1;

        if (flag == 1)
            Proc_Break(proc);
    }
}
asm(".global sub_08049E50\n.thumb_set sub_08049E50, DefeatFlow_WaitStep\n");
