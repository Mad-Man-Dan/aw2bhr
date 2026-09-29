#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A7B4.
 * sub_0806A7B4 @ 0x0806A7B4, sub_0806A8E4 @ 0x0806A8E4
 */

#include "proc.h"
#include "hardware.h"
/* One frame of a spinning/shrinking sprite proc: advance two Q12 positions by
 * their Q12 velocities, tighten the affine scale by 5 a frame, re-upload OBJ
 * affine slot 0, redraw the sprite and tick a countdown that breaks the proc.
 *
 * The angle really is the literal 0 -- gSinLut is indexed at 0 and 0x40, both
 * constants, which is SIN_Q12(0) and COS_Q12(0) folded. So the matrix is a pure
 * scale: (cos, -sin, sin, cos) * 16 / scale with sin == 0.
 *
 * `unk58 != 0 ? unk58 : 2` appears four times and re-reads the field each time
 * except the first, which reuses the value the `-= 5` store left in a register.
 * That asymmetry is CSE on the store, not two different expressions. */
struct Unk6A7B4Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ int unk44;
    /* 0x48 */ int unk48;
    /* 0x4c */ int unk4c;
    /* 0x50 */ u8 filler_50[0x08];
    /* 0x58 */ s16 unk58;
    /* 0x5a */ s16 unk5a;
    /* 0x5c */ s16 unk5c;
};
/* One frame of a scripted camera pan. +0x5a steps through the flat (dx, dy)
 * byte path gUnknown_08581608; the pair positions the map origin, the two
 * window shadows and the Q12 camera at +0x40/+0x44, and dy > 0x5a ends it.
 *
 * The whole body is skipped when bit 0 of gGameClock is set -- the frame
 * parity gate -- but the sprite redraw at the bottom runs every frame, which is
 * why the early exit is a branch to the tail and not a `return`.
 *
 * `map` is a binding local, not decoration: written as two dereferences of
 * gUnknown_08499590 the halfword store through `u16 *` may alias the pointer
 * global itself, so agbcc reloads it for the second store. The ROM loads it
 * once.
 *
 * gUnknown_030030A4's six bits are six separate `orrs`, and the last of them
 * re-uses the 0x20 already materialised for gDispIo's win0_enable rather than
 * loading it again: one CSEd constant, not two. */
struct Unk6A8E4Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ int unk44;
    /* 0x48 */ u8 filler_48[0x12];
    /* 0x5a */ s16 unk5a;
    /* 0x5c */ u16 unk5c;
    /* 0x5e */ u16 unk5e;
    /* 0x60 */ u16 unk60;
};

void MeteorImpact_FallLoop(struct Unk6A7B4Proc *proc)
{
    proc->unk40 += proc->unk48;
    proc->unk44 += proc->unk4c;
    proc->unk58 -= 5;

    SetObjAffine(0,
        Div(COS_Q12(0) * 16, proc->unk58 != 0 ? proc->unk58 : 2),
        Div(-SIN_Q12(0) * 16, proc->unk58 != 0 ? proc->unk58 : 2),
        Div(SIN_Q12(0) * 16, proc->unk58 != 0 ? proc->unk58 : 2),
        Div(COS_Q12(0) * 16, proc->unk58 != 0 ? proc->unk58 : 2));

    if ((proc->unk44 >> 12) > -0x40)
        PutOamHi(((proc->unk40 >> 12) + 0x200) & 0x1FF,
            ((proc->unk44 >> 12) + 0x100) & 0xFF, gUnknown_085815C0, 0);

    gUnknown_0300060C = proc->unk38 >> 12;
    gUnknown_0202F210 = proc->unk3c >> 12;

    proc->unk5a--;

    if (proc->unk5a < 0)
    {
        proc->unk5a = 0;
        proc->unk5c = 0x30;
        Proc_Break(proc);
    }
}
asm(".global sub_0806A7B4\n.thumb_set sub_0806A7B4, MeteorImpact_FallLoop\n");

void MeteorImpact_ShakeLoop(struct Unk6A8E4Proc *proc)
{
    int x;
    int y;
    struct Map *map;

    proc->unk5c--;

    if (proc->unk5c == 0)
        Proc_Start(gUnknown_085815E8, proc);

    if ((gGameClock & 1) == 0)
    {
        x = gUnknown_08581608[proc->unk5a * 2];
        y = gUnknown_08581608[proc->unk5a * 2 + 1];

        if (y > 0x5a)
        {
            y = 0;
            x = 0;
            Proc_Break(proc);
        }
        else
        {
            proc->unk5a++;
        }

        map = gMap;
        map->scrollX = proc->unk5e + x;
        map->scrollY = proc->unk60 + y;

        UpdateMapBgScroll();
        ResetWindowShadows();

        gDispIo.disp_ct.win0_enable = 1;

        gUnknown_030030A4.bits.win0_enable_bg0 = 1;
        gUnknown_030030A4.bits.win0_enable_bg1 = 1;
        gUnknown_030030A4.bits.win0_enable_bg2 = 1;
        gUnknown_030030A4.bits.win0_enable_bg3 = 1;
        gUnknown_030030A4.bits.win0_enable_obj = 1;
        gUnknown_030030A4.bits.win0_enable_blend = 1;

        if (x <= 0)
        {
            gUnknown_03002B40 = x < 0 ? -x : x;
            gUnknown_03002B4C = 0xf0;
        }
        else
        {
            gUnknown_03002B40 = 0;
            gUnknown_03002B4C = -0x10 - x;
        }

        if (y <= 0)
        {
            gUnknown_03002EFC = y < 0 ? -y : y;
            gUnknown_03002B44 = 0xa0;
        }
        else
        {
            gUnknown_03002EFC = 0;
            gUnknown_03002B44 = -0x60 - y;
        }

        proc->unk40 = proc->unk38 - (x << 12);
        proc->unk44 = proc->unk3c - (y << 12);
    }

    gUnknown_0300060C = proc->unk40 >> 12;
    gUnknown_0202F210 = proc->unk44 >> 12;

    PutOamHi((gUnknown_0300060C + 0x200) & 0x1FF,
        (gUnknown_0202F210 + 0x100) & 0xFF, gUnknown_085815C0, 0);
}
asm(".global sub_0806A8E4\n.thumb_set sub_0806A8E4, MeteorImpact_ShakeLoop\n");
