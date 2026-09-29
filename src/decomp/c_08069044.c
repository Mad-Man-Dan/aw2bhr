#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069044.
 * sub_08069044 @ 0x08069044
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk69044Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_IDLE_08069045(struct Unk69044Proc *proc)
{
    int i;

    switch (0xb4 - proc->unk2c)
    {
    case 0xf:
        sub_08067BD0(0, 1, 0x32, proc);
        break;

    case 0x32:
        sub_08067A24();
        break;

    case 0x40:
        for (i = 1; i < 16; i++)
            gPal[i] = 0x7fff;
        EnablePaletteSync();
        sub_080677E8();
        break;

    case 0x44:
        ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
        sub_08067D04(0, 0, 0xe, proc);
        StartIntroBgAffineTween(0, 1, 0x120, 0x80, 0, -0x4000, 0x100, 0xc0, 0xc, proc);
        break;

    case 0x74:
        for (i = 1; i < 16; i++)
            gPal[i] = 0x7fff;
        EnablePaletteSync();
        break;

    case 0x78:
        ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);
        sub_08067C7C(0x32);
        sub_0806780C();
        sub_08067A24();
        sub_08067D4C();
        break;
    }

    if (proc->unk2c != 0)
        proc->unk2c--;
    else
        Proc_Break(proc);
}

asm(".global sub_08069044\n.thumb_set sub_08069044, IntroT3_IDLE_08069045\n");
