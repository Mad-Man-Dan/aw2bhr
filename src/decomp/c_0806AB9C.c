#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AB9C.
 * sub_0806AB9C @ 0x0806AB9C
 */

#include "proc.h"
#include "hardware.h"
struct UnkAB9CProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s8 unk2c[3];
    /* 0x2f */ u8 unk2f[0x15];
    /* 0x44 */ u16 unk44;
    /* 0x46 */ u8 filler_46[0x02];
    /* 0x48 */ int unk48;
    /* 0x4c */ int unk4c;
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u16 unk52;
};

void CreditsMissionLine_Loop(struct UnkAB9CProc *proc)
{
    int i;
    u16 y;
    s16 x;
    s16 sy;

    y = (s16)proc->unk44 >> 1;

    for (i = 0; i < proc->unk48; i++)
    {
        sy = y;
        if ((u16)(sy + 0x1f) <= 0xbe)
            PutSpriteExt(0, proc->unk2f[i], (s16)(sy & 0xff), gUnknown_08581730,
                         i * 8 + proc->unk52);
    }

    sy = y;
    sy += 0x20;

    if ((u16)(sy + 0xf) <= 0xae)
        PutSprite(0, 0x3a, sy, gUnknown_08581738, 0);

    x = 0x6a;

    for (i = 0; i < 3; i++)
    {
        if (proc->unk2c[i] != -1 && (u16)(sy + 0xf) <= 0xae)
            PutSpriteExt(0, x & 0x1ff, sy & 0xff, gUnknown_08581752,
                         proc->unk2c[i] << 2);
        x += 0x10;
    }

    sy += 0x10;

    if ((u16)(sy + 0x4f) <= 0xee)
        PutSprite(0, 0x58, sy + gUnknown_085816F0[proc->unk4c].unk0c,
                  gUnknown_085816F0[proc->unk4c].unk08,
                  (proc->unk52 + 0x90) | (proc->unk50 << 12));

    proc->unk44--;

    if (((s16)proc->unk44 >> 1) < -0x7c)
        Proc_Break(proc);
}
asm(".global sub_0806AB9C\n.thumb_set sub_0806AB9C, CreditsMissionLine_Loop\n");
