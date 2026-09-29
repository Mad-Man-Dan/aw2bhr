#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DA54.
 * sub_0801DA54 @ 0x0801DA54, sub_0801DA68 @ 0x0801DA68
 */

u32 sub_0801DA54(int index)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[index];
    u32 r;

    r = p->unk2c;
    p->unk2c = 0;

    return r;
}

u8 GetSpriteScriptPriority(int index)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[index];

    return p->unk1e;
}
asm(".global sub_0801DA68\n.thumb_set sub_0801DA68, GetSpriteScriptPriority\n");
