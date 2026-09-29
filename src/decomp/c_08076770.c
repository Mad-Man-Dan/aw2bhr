#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076770.
 * sub_08076770 @ 0x08076770
 */

#include "proc.h"
struct Unk8076770
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk_2c;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ u8 filler_34[0x24];
    /* 0x58 */ s32 unk_58;
    /* 0x5c */ s32 unk_5c;
    /* 0x60 */ u8 filler_60[0x4];
    /* 0x64 */ u16 unk_64;
};

void StartWorldMapCallout(s32 a, s32 b, s32 c, ProcPtr parent)
{
    struct Unk8076770 * proc = Proc_Start(gUnknown_086144FC, parent);

    proc->unk_2c = a;
    proc->unk_30 = b;
    proc->unk_58 = c;

    proc->unk_5c = 0;
    proc->unk_64 = 0;
}
asm(".global sub_08076770\n.thumb_set sub_08076770, StartWorldMapCallout\n");
