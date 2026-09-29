#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BCF0.
 * sub_0807BCF0 @ 0x0807BCF0
 */

#include "proc.h"
struct Unk807BCF0
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x14];
    /* 0x4c */ s16 unk4c;
};

void sub_0807BCF0(ProcPtr proc)
{
    if (((struct Unk807BCF0 *)proc)->unk34 == 0)
        PlayMusicOrSfx2(0x1CF);

    sub_0807BED8(proc);

    if (((struct Unk807BCF0 *)proc)->unk34
        >= (8 - ((struct Unk807BCF0 *)proc)->unk4c) * 19 / 7 + 0x12)
        sub_0807BFB8(proc);

    if (((struct Unk807BCF0 *)proc)->unk34 > 0x31)
    {
        Proc_Start(gUnknown_08616548, proc);
        Proc_Break(proc);
    }
}
