#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049B14.
 * sub_08049B14 @ 0x08049B14
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* F002, but the callee is sub_080152C0 and NOT sub_080152EC -- the family is
 * defined by shape, and tools/families.py erases callee names from the
 * clustering key. Same one-statement body, same void epilogue.
 * The `(s32)` cast is the price of the declared `void sub_080152C0(s32, u8)`:
 * src/proc.c's ProcCmd_19_0801D214 already passes it `(s32)dataPtr`, so the
 * declaration is what the tree agrees on. It is byte-neutral (the pool word is
 * `ldr r0,=gUnknown_084C3128` either way) but it is almost certainly the wrong MODEL --
 * sub_080152C0 is the structural twin of sub_080152EC (both allocate a
 * gUnknown_03001470 slot then forward the untouched r0), every argument the
 * ROM hands it is a script blob symbol, and the same blobs are stopped by
 * sub_0801537C(const void *). Retyping it `const void *` needs one edit to
 * proc.c and a rebuild of that file to verify, which is why it was not done
 * here. */
void StartShopDrawScript(void)
{
    sub_080152C0((s32)gUnknown_084C3128, 0);
}

asm(".global sub_08049B14\n.thumb_set sub_08049B14, StartShopDrawScript\n");
