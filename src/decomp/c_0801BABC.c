#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BABC.
 * sub_0801BABC @ 0x0801BABC
 */

/* IRQ bring-up: disable everything, point all 15 handler slots at the
 * do-nothing DummyIRQRoutine, then copy crt0.s's ARM IrqMain into IWRAM and
 * publish where it landed.
 *
 * The fill loop is ASCENDING even though the ROM walks the table downwards --
 * `subs r0,#4; cmp r0,r1; bge` is what check_dbra_loop leaves behind for
 * `for (i = 0; i < 15; i++)`, and the descending spelling costs 4 bytes for a
 * counter the ROM does not keep. */
void StoreIRQToIRAM(void)
{
    int i;

    UpdateInterruptEnable(0, 0);

    for (i = 0; i < 15; i++)
        gUnknown_03002FE0[i] = DummyIRQRoutine;

    CpuFastSet(IrqMain, gUnknown_03000068, 0x40);
    gUnknown_0200BFFC = gUnknown_03000068;
}
asm(".global sub_0801BABC\n.thumb_set sub_0801BABC, StoreIRQToIRAM\n");
