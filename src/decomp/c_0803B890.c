#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B890.
 * sub_0803B890 @ 0x0803B890
 */

#include "proc.h"

/* Family F056 (data/families.json): `push {lr}; bl A; movs r0,#K; bl B;
 * pop {r0}; bx r0` -- 16 bytes, three members, and the three agree on nothing
 * but the shape: `varies` gives three distinct first callees, two distinct
 * immediates and three distinct second callees. Callee A's result (if it has
 * one) is discarded, because B's r0 is loaded by the `movs`.
 * `pop {r0}` is the void epilogue.
 */


/* The `movs r0, #3` is a PROC TREE, not a payload: sub_08034308 is
 * `void(ProcPtr parent)` in its promoted definition src/decomp/c_08034308.c,
 * where it is `Proc_StartBlocking(gUnknown_0849B8B8, parent)`. So the constant
 * is PROC_TREE_3 -- byte-identical to a bare 3, but the honest spelling.
 * BackupCampaignFlags is nullary (it loads r0 from a pool word) and void.
 */

void sub_0803B890(void)
{
    BackupCampaignFlags();
    sub_08034308(PROC_TREE_3);
}
