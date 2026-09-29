#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080360D0.
 * sub_080360D0 @ 0x080360D0
 */

#include "proc.h"
/* The proc's byte-code interpreter: fetch one opcode from the script at +0x4c,
 * dispatch, and either return (the opcode blocked) or loop for the next one.
 */

struct Unk360D0Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ struct Unk0801C210 *unk2c;
    /* 0x30 */ struct Unit *unk30;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 filler_36[0x02];
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3a */ u8 filler_3a[0x04];
    /* 0x3e */ s16 unk3e;
    /* 0x40 */ u8 filler_40[0x02];
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ u8 filler_46[0x06];
    /* 0x4c */ u8 unk4c[0x14];
};

void RunMoveSlideCommand(ProcPtr procArg)
{
    struct Unk360D0Proc *proc = procArg;
    u16 cmd;
    int t;

    for (;;)
    {
        cmd = (s8)proc->unk4c[proc->unk34++];

        switch ((s16)cmd)
        {
        case 9:
            proc->unk3e = (s8)proc->unk4c[proc->unk34++];
            proc->unk35 = 3;
            return;

        case 10:
            PlayMoveSlideStopSfx(proc);
            proc->unk35 = 5;
            sub_08027278(proc->unk42 >> 4, proc->unk44 >> 4);
            PlayMusicOrSfx2(0x7d);
            if (gPlaySt.savingEnabled != 0
                && gUnknown_030032D8 != 0x13
                && (u8)sub_0805C974() == 0)
                SendActionCommand(2, proc->unk30 - gUnits, 0, 0);
            return;

        case 4:
            StopMoveSlideWalk(proc);
            return;

        case -1:
            PlayMoveSlideStopSfx(proc);
            EndMoveSlide((struct Unk35828Proc *)proc);
            return;

        case 0:
        case 1:
        case 2:
        case 3:
            PlayMoveSlideStepSfx(proc);
            proc->unk39++;
            if ((gPlayers[gUnknown_030033EC].turnState & 2) == 0)
            {
                t = proc->unk44 + 8;
                while (t == 0)
                    t = 1;
                t = proc->unk42 + 8;
                while (t == 0)
                    t = 1;
            }
            if ((s16)cmd != proc->unk38)
            {
                AP_SwitchAnimation(proc->unk2c, (s16)cmd);
                proc->unk38 = cmd;
                proc->unk35 = 2;
            }
            return;

        case 5:
        case 6:
        case 7:
        case 8:
            cmd = (u32)(cmd * 0x10000 + 0xfffb0000) >> 16;
            if ((s16)cmd != proc->unk38)
            {
                AP_SwitchAnimation(proc->unk2c, (s16)cmd);
                proc->unk38 = cmd;
            }
            break;
        }
    }
}
asm(".global sub_080360D0\n.thumb_set sub_080360D0, RunMoveSlideCommand\n");
