#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BFB8.
 * sub_0807BFB8 @ 0x0807BFB8
 */

#include "hardware.h"
#include "proc.h"
struct Proc7BFB8
{
    u8 filler_00[0x4c];
    /* 0x4C */ s16 unk_4c;
    u8 filler_4e[0x16];
    /* 0x64 */ s16 unk_64;
};

void MissionTitleName_PutFlyingLetters(ProcPtr proc)
{
    struct Proc7BFB8 *p = proc;
    int i;
    s16 x;

    for (i = 0; i < p->unk_4c; i++)
    {
        x = p->unk_64;

        if (x > -((i + 1) * 32) && x < 0xf0 - i * 32)
            PutSpriteExt(0, ((x + ((gUnknown_0202FF78[i] * 5) >> 1)) & 0x1ff) | 0x200,
                         0x330, gUnknown_0848B6E6, i * 8 + 0x80);
    }

    p->unk_64 -= 0x18;
}
asm(".global sub_0807BFB8\n.thumb_set sub_0807BFB8, MissionTitleName_PutFlyingLetters\n");
