#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080748A0.
 * sub_080748A0 @ 0x080748A0
 */

#include "hardware.h"
#include "proc.h"
/* A proc body that draws the same sprite pair twice through two OBJ affine
 * slots, with the SECOND matrix mixing denominators the way the promoted
 * c_08027B68.c does -- pa/pc on the unit scale 0x100 and pb/pd on the
 * interpolated one -- while the FIRST uses the interpolated scale for all four.
 * The angle is the literal 0 in both (no `ands #0xff` before either gSinLut
 * load), so `COS_Q12(0) * 16` is one expression CSEd into the frame and reused
 * across all eight Div calls; that spill is why the frame is 0x10 bytes.
 */

struct Unk80748A0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a;
    /* 2c */ s16 unk2c;
    /* 2e */ s16 unk2e;
    /* 30 */ s16 unk30;
    /* 32 */ STRUCT_PAD(0x32, 0x38);
    /* 38 */ int unk38;
};

void WorldMapMarkerPopIn_Loop(struct Unk80748A0Proc *proc)
{
    int x = proc->unk2c - gUnknown_0202FDFC.unk00;
    int y = proc->unk2e - gUnknown_0202FDFC.unk02;
    int scale = Interpolate(4, 0x20, 0x100, proc->unk38, 0x10);

    SetObjAffine(0,
                 Div(COS_Q12(0) * 16, scale),
                 Div(-SIN_Q12(0) * 16, scale),
                 Div(SIN_Q12(0) * 16, scale),
                 Div(COS_Q12(0) * 16, scale));

    PutSprite(1, x, y, gUnknown_081CC4C4, 0);

    SetObjAffine(1,
                 Div(COS_Q12(0) * 16, 0x100),
                 Div(-SIN_Q12(0) * 16, scale),
                 Div(SIN_Q12(0) * 16, 0x100),
                 Div(COS_Q12(0) * 16, scale));

    PutSprite(1, x, y, gUnknown_081CC4CC, proc->unk30);

    if (scale == 0x100)
    {
        gUnknown_0202FDFC.unk12[proc->unk2a] |= 1;
        PlaySeSpacial(0x1d1, x);
        Proc_Break(proc);
    }

    proc->unk38++;
}
asm(".global sub_080748A0\n.thumb_set sub_080748A0, WorldMapMarkerPopIn_Loop\n");
