#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074714.
 * sub_08074714 @ 0x08074714, sub_0807472C @ 0x0807472C
 */

#include "proc.h"
struct Unk8074714
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ s32 unk_58;
};
struct Unk807472C
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ s32 unk_58;
};

void StartWorldMapMarkerDrawer(ProcPtr parent)
{
    ((struct Unk8074714 *)Proc_Start(gUnknown_086142B4, parent))->unk_58 = 0xc00;
}
asm(".global sub_08074714\n.thumb_set sub_08074714, StartWorldMapMarkerDrawer\n");

void SetWorldMapMarkerPriority(s32 arg)
{
    ((struct Unk807472C *)Proc_Find(gUnknown_086142B4))->unk_58 = arg << 10;
}
asm(".global sub_0807472C\n.thumb_set sub_0807472C, SetWorldMapMarkerPriority\n");
