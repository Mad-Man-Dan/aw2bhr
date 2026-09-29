#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080815C0.
 * sub_080815C0 @ 0x080815C0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* MATCHED in wave 79 (W79-A). Parked since WAVE 28 at 97.3% as a "PURE
 * REGISTER-ALLOCATION SWAP, r5 <-> r6, and nothing else": the ROM holds the
 * long-lived `(5 - i) * 2` table-index pseudo in r6 and the short-lived
 * `k = i - 4` OAM-colour pseudo in r5, and every source-level restructuring
 * tried in wave 28 (declaration order, an explicit `(j = 5 - i)` binding,
 * hoisting `k` out of the DivRem argument) left the assignment backwards.
 *
 * The whole fix is the one word `asm("r5")` below. Pinning the SHORT-LIVED
 * pseudo to the register the ROM gives it settles the tie that global_alloc
 * was resolving the other way; nothing else in the function changed. See the
 * "Three levers that move a PURE REGISTER-ALLOCATION residual" chapter of
 * docs/agbcc-codegen.md -- the same lever closed CameraScroll_Init and
 * sub_0807EEEC in the same batch. */
#include "global.h"
#include "proc.h"
struct Unk80815C0
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x48);
    /* 48 */ s16 unk48;
    /* 4a */ STRUCT_PAD(0x4a, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
};
void sub_08084700(struct Unk80815C0 *);

void MainMenuCarouselWheel_TilesSlideInLoop(struct Unk80815C0 *proc)
{
    int i;
    register int k asm("r5");

    if (proc->unk4c <= 0x2d)
        proc->unk48 = Interpolate(4, 0, 0x30, proc->unk4c, 0x30);

    for (i = 0; i <= 4; i++)
    {
        if (Div(proc->unk48, 9) >= i && Div(proc->unk48, 9) < i + 1)
        {
            proc->unk2c = Interpolate(4, -0x90, gUnknown_08616972[5 - i],
                                      DivRem(proc->unk48, 9), 9);
            PutSprite(2, proc->unk2c & 0x1FF, gUnknown_08616980[5 - i],
                      gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(proc->unk52 - (k = i - 4), 6)] + 2) << 12)
                          | 0x800
                          | (gUnknown_0861696C[DivRem(proc->unk52 - k, 6)] * 32 + 0x1D8));
        }
        else if (Div(proc->unk48, 9) >= i + 1)
        {
            PutSprite(2, gUnknown_08616972[5 - i] & 0x1FF, gUnknown_08616980[5 - i],
                      gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(proc->unk52 - (k = i - 4), 6)] + 2) << 12)
                          | 0x800
                          | (gUnknown_0861696C[DivRem(proc->unk52 - k, 6)] * 32 + 0x1D8));
        }
    }

    if (proc->unk4c <= 0x2c)
        proc->unk4c++;
    else
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }

    sub_08084700(proc);
}

asm(".global sub_080815C0\n.thumb_set sub_080815C0, MainMenuCarouselWheel_TilesSlideInLoop\n");
