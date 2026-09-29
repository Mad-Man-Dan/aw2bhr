#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C78C.
 * sub_0806C78C @ 0x0806C78C
 */

#include "proc.h"

/* Family F056 (data/families.json): `push {lr}; bl A; movs r0,#K; bl B;
 * pop {r0}; bx r0` -- 16 bytes, three members, and the three agree on nothing
 * but the shape: `varies` gives three distinct first callees, two distinct
 * immediates and three distinct second callees. Callee A's result (if it has
 * one) is discarded, because B's r0 is loaded by the `movs`.
 * `pop {r0}` is the void epilogue.
 */


/* Proc_Init is proc.h's own `void(void)`, so nothing flows from it into the
 * second call. StartTitleScreen is `void(s32)` in src/title-screen.c -- upstream's
 * matching source, which must not be edited; the type is copied from there.
 */

void ResetProcsAndStartTitleScreen(void)
{
    Proc_Init();
    StartTitleScreen(1);
}
asm(".global sub_0806C78C\n.thumb_set sub_0806C78C, ResetProcsAndStartTitleScreen\n");
