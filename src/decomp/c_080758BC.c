#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080758BC.
 * sub_080758BC @ 0x080758BC
 */

#include "proc.h"
struct Unk80758BC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk_2c;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ u16 unk_34;
    /* 0x36 */ u16 filler_36;
    /* 0x38 */ s32 unk_38;
    /* 0x3c */ s32 unk_3c;
    /* 0x40 */ s32 unk_40;
};

void StartWorldMapReticle(s32 a, s32 b, s32 c, ProcPtr parent)
{
    struct Unk80758BC * proc = Proc_Start(gUnknown_086143E0, parent);

    proc->unk_2c = a;
    proc->unk_30 = b;
    proc->unk_38 = c;

    proc->unk_34 = 0;
    proc->unk_3c = 0;
    proc->unk_40 = 0;
}
asm(".global sub_080758BC\n.thumb_set sub_080758BC, StartWorldMapReticle\n");
