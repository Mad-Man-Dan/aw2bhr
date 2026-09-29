#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040624.
 * sub_08040624 @ 0x08040624
 */

#include "proc.h"

/* A four-parameter wrapper over StartSiloMissileFall that fixes the third and fourth
 * arguments at 0x1CA and 5 -- the same pair StartSiloLaunch passes to
 * StartSiloMissileLaunch, the other proc of the family. 0x1CA is spelled as a literal:
 * agbcc builds it `movs #0xe5; lsls #1`, which is the ROM's two instructions. */
void StartSiloMissileStrike(int a, int b, int c, ProcPtr parent)
{
    StartSiloMissileFall(a, b, 0x1CA, 5, c, parent);
}
asm(".global sub_08040624\n.thumb_set sub_08040624, StartSiloMissileStrike\n");
