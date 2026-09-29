#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AF70.
 * sub_0803AF70 @ 0x0803AF70
 */

struct Unk0803AF70
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u16 unk1e;
    /* 0x20 */ u16 unk20;
};

void DebugBackupUtility_Init(struct Unk0803AF70 *p)
{
    p->unk1e = 0;
    p->unk20 = 0;
}
asm(".global sub_0803AF70\n.thumb_set sub_0803AF70, DebugBackupUtility_Init\n");
