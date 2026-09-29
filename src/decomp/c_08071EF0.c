#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071EF0.
 * sub_08071EF0 @ 0x08071EF0, sub_08071F0C @ 0x08071F0C, sub_08071F28 @ 0x08071F28, sub_08071F40 @ 0x08071F40, sub_08071F58 @ 0x08071F58, sub_08071F70 @ 0x08071F70
 */

#include "proc.h"
struct Unk8613E64Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk8613E84Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk8613EA4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk8613EC4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

void StartFadeToBlackUnused(int a)
{
    ((struct Unk8613E64Proc *)Proc_Start(gUnknown_08613E64, PROC_TREE_3))->unk64 = a;
}
asm(".global sub_08071EF0\n.thumb_set sub_08071EF0, StartFadeToBlackUnused\n");

void StartFadeFromBlackUnused(int a)
{
    ((struct Unk8613E84Proc *)Proc_Start(gUnknown_08613E84, PROC_TREE_3))->unk64 = a;
}
asm(".global sub_08071F0C\n.thumb_set sub_08071F0C, StartFadeFromBlackUnused\n");

void StartLockingFadeToBlackUnused(int a, ProcPtr parent)
{
    ((struct Unk8613E64Proc *)Proc_StartBlocking(gUnknown_08613E64, parent))->unk64 = a;
}
asm(".global sub_08071F28\n.thumb_set sub_08071F28, StartLockingFadeToBlackUnused\n");

void StartLockingFadeFromBlackUnused(int a, ProcPtr parent)
{
    ((struct Unk8613E84Proc *)Proc_StartBlocking(gUnknown_08613E84, parent))->unk64 = a;
}
asm(".global sub_08071F40\n.thumb_set sub_08071F40, StartLockingFadeFromBlackUnused\n");

void StartLockingFadeToWhiteUnused(int a, ProcPtr parent)
{
    ((struct Unk8613EA4Proc *)Proc_StartBlocking(gUnknown_08613EA4, parent))->unk64 = a;
}
asm(".global sub_08071F58\n.thumb_set sub_08071F58, StartLockingFadeToWhiteUnused\n");

void StartLockingFadeFromWhiteUnused(int a, ProcPtr parent)
{
    ((struct Unk8613EC4Proc *)Proc_StartBlocking(gUnknown_08613EC4, parent))->unk64 = a;
}
asm(".global sub_08071F70\n.thumb_set sub_08071F70, StartLockingFadeFromWhiteUnused\n");
