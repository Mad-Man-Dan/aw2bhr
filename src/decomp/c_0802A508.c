#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A508.
 * sub_0802A508 @ 0x0802A508, sub_0802A514 @ 0x0802A514, sub_0802A528 @ 0x0802A528, sub_0802A538 @ 0x0802A538
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Family F003 (data/families.json): 34 twelve-byte forwarders shaped
 * `push {lr}; movs r0,#K; bl S; pop {r0}; bx r0`, i.e. one call with one
 * literal argument and nothing else. `pop {r0}; bx r0` fixes this as void.
 * The argument's C type is not recoverable from here -- `movs r0,#K` is the
 * same two bytes for int/u32/u16/s16/u8 -- so the spelling follows the
 * callee's declaration in unknown-functions.h and nothing else.
 * Exemplar: src/decomp/c_08004A60.c. */

void sub_0802A508(void)
{
    SetInfoBoxMode(0);
}

/* F002, but the callee is sub_080152C0 and NOT sub_080152EC -- the family is
 * defined by shape, and tools/families.py erases callee names from the
 * clustering key. Same one-statement body, same void epilogue.
 * The `(s32)` cast is the price of the declared `void sub_080152C0(s32, u8)`:
 * src/proc.c's ProcCmd_19_0801D214 already passes it `(s32)dataPtr`, so the
 * declaration is what the tree agrees on. It is byte-neutral (the pool word is
 * `ldr r0,=gUnknown_0849A108` either way) but it is almost certainly the wrong MODEL --
 * sub_080152C0 is the structural twin of sub_080152EC (both allocate a
 * gUnknown_03001470 slot then forward the untouched r0), every argument the
 * ROM hands it is a script blob symbol, and the same blobs are stopped by
 * sub_0801537C(const void *). Retyping it `const void *` needs one edit to
 * proc.c and a rebuild of that file to verify, which is why it was not done
 * here. */
void sub_0802A514(void)
{
    sub_080152C0((s32)gUnknown_0849A108, 1);
}

/* Not a proc script: gUnknown_0849A108 belongs to the gUnknown_03001470 list,
 * whose start half is sub_0802A514 (`sub_080152C0(gUnknown_0849A108, 1)`) and
 * whose stop half is this. sub_0801537C returns an s8 slot index; `pop {r0}`
 * says this wrapper discards it, and a discarded return emits no narrowing, so
 * this call site does not constrain the width either way.
 */

void sub_0802A528(void)
{
    sub_0801537C(gUnknown_0849A108);
}

/* F002, but the callee is sub_080152C0 and NOT sub_080152EC -- the family is
 * defined by shape, and tools/families.py erases callee names from the
 * clustering key. Same one-statement body, same void epilogue.
 * The `(s32)` cast is the price of the declared `void sub_080152C0(s32, u8)`:
 * src/proc.c's ProcCmd_19_0801D214 already passes it `(s32)dataPtr`, so the
 * declaration is what the tree agrees on. It is byte-neutral (the pool word is
 * `ldr r0,=gUnknown_0849A128` either way) but it is almost certainly the wrong MODEL --
 * sub_080152C0 is the structural twin of sub_080152EC (both allocate a
 * gUnknown_03001470 slot then forward the untouched r0), every argument the
 * ROM hands it is a script blob symbol, and the same blobs are stopped by
 * sub_0801537C(const void *). Retyping it `const void *` needs one edit to
 * proc.c and a rebuild of that file to verify, which is why it was not done
 * here. */
void StartTurnStartRepairAndSupply(void)
{
    sub_080152C0((s32)gUnknown_0849A128, 0);
}
asm(".global sub_0802A538\n.thumb_set sub_0802A538, StartTurnStartRepairAndSupply\n");
