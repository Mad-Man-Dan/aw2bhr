#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B3E8.
 * sub_0806B3E8 @ 0x0806B3E8
 */

#include "proc.h"
#include "hardware.h"
struct UnkB3E8Proc
{
    /* 0x00 */ u8 filler_00[0x5c];
    /* 0x5c */ int unk5c;
};

void CreditsEpilogue_FadeToGrey(struct UnkB3E8Proc *proc)
{
    int i;
    int a;
    int b;
    u16 cr;
    u16 cg;
    u16 cb;
    u16 y;

    a = Interpolate(0, 0x10, 0xa, proc->unk5c, 0x10);
    b = Interpolate(0, 0, -2, proc->unk5c, 0x10);

    ColFadeDirect((u16 *)gUnknown_0823BDE0, 0, 1, 1);
    ColFadeDirect(gUnknown_08194280, 4, 1, 1);

    for (i = 0; i < 0x10; i++)
    {
        cr = (s8)gUnknown_0200B614[i * 3];
        cg = (s8)gUnknown_0200B614[i * 3 + 1];
        cb = (s8)gUnknown_0200B614[i * 3 + 2];
        y = (cr * 3 + cg * 6 + cb) / 10;
        cr = Interpolate(0, cr, y, proc->unk5c, 0x10);
        cg = Interpolate(0, cg, y, proc->unk5c, 0x10);
        cb = Interpolate(0, cb, y, proc->unk5c, 0x10);
        gPal[i] = cr + ((cb << 10) + (cg << 5));
    }

    for (i = 0x40; i < 0x50; i++)
    {
        cr = (s8)gUnknown_0200B614[i * 3];
        cg = (s8)gUnknown_0200B614[i * 3 + 1];
        cb = (s8)gUnknown_0200B614[i * 3 + 2];
        y = (cr * 3 + cg * 6 + cb) / 10;
        cr = Interpolate(0, cr, y, proc->unk5c, 0x10);
        cg = Interpolate(0, cg, y, proc->unk5c, 0x10);
        cb = Interpolate(0, cb, y, proc->unk5c, 0x10);
        gPal[i] = cr + ((cb << 10) + (cg << 5));
    }

    EnablePaletteSync();
    SetBgScrollShadow(3, 0, 0);
    SetBgScrollShadow(2, b, b);

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = (a >> 1) + (a & 1);
    gUnknown_03002B28 = a >> 1;
    gUnknown_03001FFC = 0;

    if (proc->unk5c > 0xf)
        Proc_Break(proc);
    else
        proc->unk5c++;
}
asm(".global sub_0806B3E8\n.thumb_set sub_0806B3E8, CreditsEpilogue_FadeToGrey\n");
