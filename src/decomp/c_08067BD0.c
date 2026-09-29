#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067BD0.
 * sub_08067BD0 @ 0x08067BD0
 */

#include "hardware.h"
#include "proc.h"
struct Unk8067BD0Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
};

void StartIntroSlidePanel(int a, int b, int c, ProcPtr parent)
{
    struct Unk8067BD0Proc *proc;

    proc = Proc_Start(gUnknown_08581068, parent);

    proc->unk30 = b << 7;
    proc->unk34 = b;
    proc->unk2c = a;
    proc->unk38 = c;
    proc->unk3c = 0;

    ApplyPaletteExt(gUnknown_08183780, 0xa0, 0x20);
    Decompress(gUnknown_08581044[a], (void *)0x06000000);

    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xffe0) | 2;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xe0ff) | 0x100;

    SetBgScrollShadow(1, proc->unk30, 0);
}
asm(".global sub_08067BD0\n.thumb_set sub_08067BD0, StartIntroSlidePanel\n");
