#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BE24.
 * sub_0807BE24 @ 0x0807BE24
 */

struct UnkProc7BE24
{
    u8 filler_00[0x4c];
    u16 unk_4c;
    u8 filler_4e[0x60 - 0x4e];
    u32 unk_60;
    u8 filler_64[0x66 - 0x64];
    u16 unk_66;
    u16 unk_68;
};

void MissionTitleLetters_Init(struct UnkProc7BE24 *proc)
{
    proc->unk_4c = gUnknown_0300592C;
    proc->unk_66 = 0;
    proc->unk_68 = 0;
    proc->unk_60 = 0;
}
asm(".global sub_0807BE24\n.thumb_set sub_0807BE24, MissionTitleLetters_Init\n");
