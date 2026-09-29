#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080805E0.
 * sub_080805E0 @ 0x080805E0, sub_080806B0 @ 0x080806B0
 */

#include "hardware.h"
#include "proc.h"
struct Unk080805E0
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u8 filler_4e[0x58 - 0x4e];
    /* 0x58 */ int unk58;
};
struct Unk080806B0
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x4c - 0x30];
    /* 0x4c */ s16 unk4c;
};

void SuperCoPowerName_Init(struct Unk080805E0 *proc)
{
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = gUnknown_03001FFC;
    *(u16 *)&gUnknown_030030E0 = *(u16 *)&gUnknown_030030E0 & 0xFFE0;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x800;
    gUnknown_030030E0.bits.target2_enable_bd = 0;
    gUnknown_030030DC.bits.win0_enable_blend = 0;
    gUnknown_030030DC.bits.win1_enable_blend = 0;

    proc->unk58 = LoadCoPowerNameLetters(
        gTextTable[gUnknown_085D3DD0[gUnknown_03005970].power[gUnknown_03005904].powerNameId],
        gUnknown_08616750, proc);
    proc->unk4c = 0;
}
asm(".global sub_080805E0\n.thumb_set sub_080805E0, SuperCoPowerName_Init\n");

void SuperCoPowerName_SlideInLoop(struct Unk080806B0 *proc)
{
    if (proc->unk4c < 0x30)
    {
        proc->unk2c = Interpolate(4, 0x38, 0xA8, proc->unk4c, 0x30);
        gUnknown_03002020 = Interpolate(0, 0, 0x10, proc->unk4c, 0x30);
        gUnknown_03002B28 = Interpolate(0, 0x10, 0, proc->unk4c, 0x30);
        sub_08043C28(proc->unk2c & 0x1FF, 0x4A0, 0x11CA, 0, 1);
    }
    else if (proc->unk4c == 0x30)
    {
        sub_08043C28(0xA8, 0xA0, 0x11CA, 0, 1);
        Proc_Start(gUnknown_0861693C, proc);
    }
    else if (proc->unk4c < 0x40)
    {
        sub_08043C28(0xA8, 0xA0, 0x11CA, 0, 1);
    }
    else
    {
        sub_08043C28(0xA8, 0xA0, 0x11CA, 0, 1);
        proc->unk4c = 0;
        Proc_Break(proc);
    }

    proc->unk4c++;
}
asm(".global sub_080806B0\n.thumb_set sub_080806B0, SuperCoPowerName_SlideInLoop\n");
