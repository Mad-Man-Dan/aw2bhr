#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074B60.
 * sub_08074B60 @ 0x08074B60
 */

#include "proc.h"
struct Unk08074B60
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 filler_34[0x38 - 0x34];
    /* 0x38 */ s16 unk38;
    /* 0x3a */ s16 unk3a;
    /* 0x3c */ int unk3c;
};

void WorldMapCameraPan_Loop(struct Unk08074B60 *proc)
{
    if (proc->unk3c == 0)
    {
        proc->unk2c = gUnknown_0202FDFC.unk00;
        proc->unk2e = gUnknown_0202FDFC.unk02;
        Proc_End(proc);
    }
    else
    {
        proc->unk3a -= gUnknown_0202FEF8[proc->unk3c];
        proc->unk3c--;

        gUnknown_0202FDFC.unk00 =
            proc->unk2c + ((proc->unk30 - proc->unk2c) * proc->unk3a) / proc->unk38;
        gUnknown_0202FDFC.unk02 =
            proc->unk2e + ((proc->unk32 - proc->unk2e) * proc->unk3a) / proc->unk38;

        SetBgScrollShadow(3, gUnknown_0202FDFC.unk00, gUnknown_0202FDFC.unk02);
    }
}
asm(".global sub_08074B60\n.thumb_set sub_08074B60, WorldMapCameraPan_Loop\n");
