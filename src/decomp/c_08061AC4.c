#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061AC4.
 * sub_08061AC4 @ 0x08061AC4
 */

/* Two guards sharing one exit. gUnknown_030044D8 == 1 skips everything;
 * otherwise the teardown runs and unk32 gates the last call.
 *
 * Both struct members are reached with adds r0, #0x2e / #0x32 on a COPY of the
 * base register rather than a ldrb displacement -- ldrb's imm5 stops at 31 --
 * and the base pool word is loaded once and held, which is why the copy exists
 * at all. */
void AiEndTurnStep(void)
{
    if (gUnknown_030044D8 != 1)
    {
        EndCurrentArmyTurn();
        gUnknown_03004780 = 0;

        if (gPlaySt.savingEnabled != 0)
            sub_080344F0(gPlaySt.unk2e);
    }
}
asm(".global sub_08061AC4\n.thumb_set sub_08061AC4, AiEndTurnStep\n");
