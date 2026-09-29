#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080364D4.
 * sub_080364D4 @ 0x080364D4, sub_080364E0 @ 0x080364E0
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
 * One pointer passed through to HeapFree(void *), the heap free.
 * Confirmed at the call site: sub_080363D0 does `ldr r0,[r4,#0x48]`
 * immediately before `bl FreeMoveSlideGfxBuffer`.
 */
void FreeMoveSlideGfxBuffer(void * a)
{
    HeapFree(a);
}
asm(".global sub_080364D4\n.thumb_set sub_080364D4, FreeMoveSlideGfxBuffer\n");

/* F002, but the callee is sub_080152C0 and NOT sub_080152EC -- the family is
 * defined by shape, and tools/families.py erases callee names from the
 * clustering key. Same one-statement body, same void epilogue.
 * The `(s32)` cast is the price of the declared `void sub_080152C0(s32, u8)`:
 * src/proc.c's ProcCmd_19_0801D214 already passes it `(s32)dataPtr`, so the
 * declaration is what the tree agrees on. It is byte-neutral (the pool word is
 * `ldr r0,=gUnknown_0849D10C` either way) but it is almost certainly the wrong MODEL --
 * sub_080152C0 is the structural twin of sub_080152EC (both allocate a
 * gUnknown_03001470 slot then forward the untouched r0), every argument the
 * ROM hands it is a script blob symbol, and the same blobs are stopped by
 * sub_0801537C(const void *). Retyping it `const void *` needs one edit to
 * proc.c and a rebuild of that file to verify, which is why it was not done
 * here. */
void sub_080364E0(void)
{
    sub_080152C0((s32)gUnknown_0849D10C, 0);
}
