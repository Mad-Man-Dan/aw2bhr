#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AA08.
 * sub_0803AA08 @ 0x0803AA08
 */

/* src/decomp/c_0803AA78.c's other twin, and the only structural difference is
 * what the two UnitInfoPanel_LoadPictureDrawMoveAndVision / sub_0803A2BC calls are handed: RefreshDeploymentUnitInfo
 * passes &gUnknown_03004100 itself, this one passes the unk18 word of the
 * gUnknown_03001470 slot that FindSlotScript just found.
 *
 * The `lsls #0x18; asrs #8; lsrs #0x10` triple is ONE narrowing, not a mask
 * plus a shift: left 24, arithmetic right 8, logical right 16 leaves the
 * sign-extended byte zero-extended into 16 bits, i.e. `(u16)(s8)`. That is an
 * s16 LOCAL holding FindSlotScript's s8 return and living across the
 * InitUnit call -- the wave-29 (W29-C) round trip -- which is why the use
 * re-signs it with `lsls #0x10; asrs #0x10` before the x0x60 index scaling. */

void sub_0803AA08(u8 a)
{
    s16 slot = FindSlotScript((s32)gUnknown_0849E240);

    InitUnit(&gUnknown_03004100, a);

    gUnknown_0849D89C->unk04 = &gUnknown_03004100;

    UnitInfoPanel_LoadPictureDrawMoveAndVision(gUnknown_0849D89C->unk00,
        (struct Unit *)gUnknown_03001470[slot].unk18);
    sub_0803A2BC(gUnknown_0849D89C->unk00,
        (struct Unit *)gUnknown_03001470[slot].unk18);

    sub_08013AD4(0);
}
