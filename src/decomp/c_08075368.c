#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075368.
 * sub_08075368 @ 0x08075368
 */

#include "hardware.h"
/* A four-arm `switch (proc->unk38)` fade/blend driver: arm 1 ramps the two
 * blend weights in with Interpolate kind 4 over 0x10 frames and hands off to
 * arm 2, arm 2 just spins the 5-bit palette phase, arm 3 ramps back out with
 * kind 0 over 8 frames, and arm 0 tears the blend down. The dispatch is gcc's
 * balanced decision tree for a four-case switch (`cmp #1 / beq / cmp #1 / bgt`),
 * not an if-chain.
 *
 * A data_refs-subset target and the one W20-B's pre-registration expected to be
 * dearest: 13 branches and the only undeclared callee of its batch
 * (SetWorldMapSelectionFrameColor, now in include/unknown-functions.h). It cost one attempt, the
 * same as the 180-byte straight-line sibling -- see docs/agbcc-codegen.md for
 * why that kills the branches-plus-undeclared-callees proxy.
 *
 * Two things are cse's `record_jump_equiv` rather than source: arm 0 stores the
 * still-live comparison zero into gUnknown_03002B28 and gUnknown_03001FFC
 * (`strh r3` where r3 is proc->unk38), and arm 1's `SetWorldMapSelectionFrameColor(proc->unk40)`
 * reuses the 0x18 it just stored instead of reloading. `gUnknown_030030E0.bits
 * .effect = 0` is one `ldrb; and #0x3f; strb` because the two bits are adjacent
 * -- the same fold the wave-19 bitfield rule describes, here wanted rather than
 * fought. */
#include "proc.h"
struct Unk8075368
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ s16 unk2e;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 filler_36[0x02];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
};

void WorldMapSelectionFrame_Loop(struct Unk8075368 *proc)
{
    switch (proc->unk38)
    {
    case 0:
        gUnknown_030030E0.bits.effect = 0;
        gUnknown_03002020 = 0x10;
        gUnknown_03002B28 = 0;
        gUnknown_03001FFC = 0;
        break;

    case 1:
        proc->unk2a = Interpolate(4, proc->unk2e, proc->unk32, proc->unk3c, 0x10);
        proc->unk2c = Interpolate(4, proc->unk30, proc->unk34, proc->unk3c, 0x10);
        SetBgScrollShadow(1, proc->unk2a, proc->unk2c);
        proc->unk40 = 0x18;
        SetWorldMapSelectionFrameColor(proc->unk40);

        if (proc->unk3c == 0x10)
        {
            proc->unk38 = 2;
            proc->unk2e = proc->unk2a;
            proc->unk30 = proc->unk2c;
            PlaySeSpacial(0x1DC, proc->unk3c - proc->unk32);
        }

        proc->unk3c++;
        break;

    case 2:
        proc->unk40 = (proc->unk40 + 1) & 0x1F;
        SetWorldMapSelectionFrameColor(proc->unk40);
        break;

    case 3:
        proc->unk2a = Interpolate(0, proc->unk2e, proc->unk32, proc->unk3c, 8);
        proc->unk2c = Interpolate(0, proc->unk30, proc->unk34, proc->unk3c, 8);
        SetBgScrollShadow(1, proc->unk2a, proc->unk2c);

        if (proc->unk3c == 8)
        {
            proc->unk38 = 0;
            proc->unk2e = proc->unk2a;
            proc->unk30 = proc->unk2c;
        }

        if (proc->unk40 != 0x18)
            proc->unk40++;

        proc->unk40 &= 0x1F;
        SetWorldMapSelectionFrameColor(proc->unk40);

        proc->unk3c++;
        break;
    }
}
asm(".global sub_08075368\n.thumb_set sub_08075368, WorldMapSelectionFrame_Loop\n");
