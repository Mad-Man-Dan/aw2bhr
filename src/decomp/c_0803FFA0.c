#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803FFA0.
 * sub_0803FFA0 @ 0x0803FFA0
 */

#include "proc.h"
struct UnkFFA0Proc
{
    PROC_HEADER;
    STRUCT_PAD(0x29, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
};

void ExplosionEffect_Init(struct UnkFFA0Proc *proc)
{
    u8 *blob;
    u16 *palette;
    void *descriptor;

    switch (proc->unk54)
    {
    case -3:
    default:
        blob = gUnknown_0810EB00;
        palette = gUnknown_0810F364;
        descriptor = gUnknown_0810F384;
        break;
    case 2:
    case 3:
        blob = gUnknown_0810F410;
        palette = gUnknown_0810FAE8;
        descriptor = gUnknown_0810FB08;
        break;
    case 4:
        blob = gUnknown_0810FB94;
        palette = gUnknown_0810FF4C;
        descriptor = gUnknown_0810FF6C;
        break;
    case -2:
    case -1:
        blob = gUnknown_0810FFE0;
        palette = gUnknown_08110CBC;
        descriptor = gUnknown_08110CDC;
        break;
    }
    proc->unk50 = AP_Create(descriptor, 1, 1);
    proc->unk50->unk22 = 0x51CA;
    if (proc->unk54 != -2)
    {
        if (proc->unk54 == -1)
        {
            proc->unk50->unk18 = 0;
            proc->unk50->unk1a = Div(0x7800, 0x64);
        }
    }
    else
    {
        proc->unk50->unk18 = 0;
        proc->unk50->unk1a = Div(0xC800, 0x64);
    }
    Decompress(blob, (void *)0x06013940);
    ApplyPaletteExt(palette, 0x2A0, 0x20);
}
asm(".global sub_0803FFA0\n.thumb_set sub_0803FFA0, ExplosionEffect_Init\n");
