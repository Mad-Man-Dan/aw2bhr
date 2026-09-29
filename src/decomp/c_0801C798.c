#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C798.
 * sub_0801C798 @ 0x0801C798, sub_0801C7A4 @ 0x0801C7A4
 */

#include "proc.h"

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * One pointer passed through to Proc_End(ProcPtr). Unreferenced anywhere in
 * the ROM, so the arity is the callee's and nothing else.
 */
void APProc_Delete(ProcPtr proc)
{
    Proc_End(proc);
}
asm(".global sub_0801C798\n.thumb_set sub_0801C798, APProc_Delete\n");

/* ProcScr_WaitForLaser was already typed `const struct ProcCmd []` by
 * APProc_Exists, the "is it running" predicate on the same script.
 */

void APProc_DeleteAll(void)
{
    Proc_EndEach(ProcScr_WaitForLaser);
}
asm(".global sub_0801C7A4\n.thumb_set sub_0801C7A4, APProc_DeleteAll\n");
