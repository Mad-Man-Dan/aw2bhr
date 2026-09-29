#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D168.
 * sub_0802D168 @ 0x0802D168, sub_0802D1A0 @ 0x0802D1A0, sub_0802D1C0 @ 0x0802D1C0, sub_0802D1F8 @ 0x0802D1F8
 */

/* An if/else and NOT `g = (cond != 0)`: two separate `strb`s split across an
 * unconditional `b`, which is the spelling `return <cmp>` / a stored comparison
 * cannot produce (docs/agbcc-codegen.md -- a stored flag goes through
 * do_store_flag and arrives with no unconditional branch). The else arm reuses
 * the register the guard already loaded the zero into, hence `strb r1`.
 *
 * gUnknown_030033E8 is declared `u8 []` (two bytes are known written); this
 * reads element 0. The two pool words for gUnknown_03000558 are -fforce-addr
 * giving each arm its own copy. */

void sub_0802D168(void)
{
    if (gUnknown_030033E8[0] != 0)
        gUnknown_03000558 = 1;
    else
        gUnknown_03000558 = 0;

    sub_08029948(0);
    CloseTopMenu();
    IncrementMapLock();
}

/* The unconditional twin of sub_0802D168: the flag is forced to 1 and the
 * sub_08029948 argument goes with it. The two `movs r0,#1` are not CSEd
 * because the `strb` and the argument are separate values to gcc. */

void sub_0802D1A0(void)
{
    gUnknown_03000558 = 1;

    sub_08029948(1);
    CloseTopMenu();
    IncrementMapLock();
}

/* See src/decomp/c_0802D064.c: same shape with two of the six calls dropped,
 * sub_08060684 in the middle and command id 0xb. */

void sub_0802D1C0(void)
{
    LockUnitSelection();
    CloseTopMenu();
    sub_08060684();
    CommitUnitMove();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0xB, gUnknown_03003F38, 0, 0);
}

/* See src/decomp/c_0802D064.c and c_0802D1C0.c: the sub_080606A0 twin of
 * sub_0802D1C0, command id 0xc. */

void sub_0802D1F8(void)
{
    LockUnitSelection();
    CloseTopMenu();
    sub_080606A0();
    CommitUnitMove();

    if (gPlaySt.savingEnabled != 0)
        SendActionCommand(0xC, gUnknown_03003F38, 0, 0);
}
