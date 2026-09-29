#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804438C.
 * sub_0804438C @ 0x0804438C
 */

void PayForCoPower(int a1, int a2)
{
    IncrementCoPowerUseCount(a1);
    SpendCoPowerCharge(a1, a2);
    gPlayers[a1].unk24 = 0;
    StartCoPowerSequence(a1, a2);
}
asm(".global sub_0804438C\n.thumb_set sub_0804438C, PayForCoPower\n");
