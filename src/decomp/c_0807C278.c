#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C278.
 * sub_0807C278 @ 0x0807C278
 */

#include "proc.h"
#include "hardware.h"
struct Unk807C278
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
};

void MissionTitleLetters_PutLetterRow(void *arg)
{
    struct Unk807C278 *proc = arg;
    int i;

    for (i = 0; i < proc->unk4c; i++)
        PutSprite(0, gUnknown_0202FF78[i] - gUnknown_0202FF78[proc->unk4c] + 0xE0,
                  0x18, gUnknown_0848B6E6, i * 8 + 0x80);
}
asm(".global sub_0807C278\n.thumb_set sub_0807C278, MissionTitleLetters_PutLetterRow\n");
