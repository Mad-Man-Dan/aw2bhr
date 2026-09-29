#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AF44.
 * sub_0806AF44 @ 0x0806AF44
 */

#include "hardware.h"
#include "proc.h"
struct Unk806AF44Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
};
struct Unk806AF44Child
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ void *unk30;
    /* 0x34 */ u16 *unk34;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
};

int StartCreditsIllustration(ProcPtr a1)
{
    struct Unk806AF44Child *child;
    int unit;

    unit = gUnknown_085C77A0[
        gUnknown_0202F214[((struct Unk806AF44Proc *)a1)->unk2a].unk00].unk16;

    if (unit == 0)
        return 0;

    unit--;

    child = Proc_Start(gUnknown_085818F4, a1);
    child->unk2c = unit;

    if ((((struct Unk806AF44Proc *)a1)->unk2c & 1) == 0)
    {
        child->unk30 = (void *)(0x06000000 + gUnknown_0300251C.bits.chr_block * 0x4000);
        child->unk34 = gBG3TilemapBuffer;
        child->unk38 = 0;
        child->unk39 = 3;
    }
    else
    {
        child->unk30 = (void *)(0x06000000 + gUnknown_03001FE8.bits.chr_block * 0x4000);
        child->unk34 = gBG1TilemapBuffer;
        child->unk38 = 7;
        child->unk39 = 1;
    }

    return 1;
}
asm(".global sub_0806AF44\n.thumb_set sub_0806AF44, StartCreditsIllustration\n");
