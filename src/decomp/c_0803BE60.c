#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BE60.
 * sub_0803BE60 @ 0x0803BE60
 */

/* MATCHED byte-for-byte (wave 37, W37-Q2).
 *
 * Three counted u8 loops: mark slots 1..n live, slots n+1..4 dead, then give
 * every slot 100000 of whatever unk10 counts.
 *
 * `unk10` had to become a real member ARRAY for this to match, and that is the
 * whole derivation. The ROM keeps the UNBIASED `.word gPlaySt` in
 * r2, hoists `adds r5,r2,#0; adds r5,#0x10` into the third preheader and
 * reaches unk28 as `str r3,[r2,#0x28]`. Two rejected spellings, both 4 bytes
 * short:
 *   - `(&gPlaySt.unk14)[i - 1]` on the old scalar fields folds the
 *     offset into the pool word (`.word gPlaySt+0x10`) and then
 *     reaches unk28 as `[r2,#0x18]` off that biased base -- no hoisted add at
 *     all.
 *   - binding a `struct PlaySt *p = &gPlaySt;` local gets the
 *     unbiased base but folds the 0x10 into the store displacement
 *     (`str r3,[r0,#0x10]`) and pulls the `ldr` up to the top of the function,
 *     ahead of the gUnknown_0849ECDC load, where the ROM has it in the third
 *     preheader.
 * Only the member-array spelling on the global gives both the unbiased base
 * and the hoisted +0x10. This is the "p->arr[i] hoists where pointer
 * arithmetic folds" rule in docs/agbcc-codegen.md, and it discriminates a
 * declared array from four scalars at the same addresses.
 *
 * `gPlaySt.propertyFunds = 1000` is INSIDE the loop in the source: it is
 * re-stored on every iteration and agbcc does not sink stores.
 *
 * `i = n + 1` on a u8 gives the `adds r0,r2,#1; lsls #0x18; lsrs r1,#0x18`
 * that opens the second loop; the second loop's `movs r2,#0` then kills n,
 * which is safe because n is not read again. */

void Versus_InitArmyRoster(void)
{
    u8 i;
    u8 n;

    n = gUnknown_0849ECDC->unk00;

    for (i = 1; i <= n; i++)
        gUnknown_03003FF8[i] = 1;

    for (i = n + 1; i <= 4; i++)
        gUnknown_03003FF8[i] = 0;

    for (i = 1; i <= 4; i++)
    {
        gPlaySt.unk10[i] = 100000;
        gPlaySt.propertyFunds = 1000;
    }
}
asm(".global sub_0803BE60\n.thumb_set sub_0803BE60, Versus_InitArmyRoster\n");
