#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017BD4.
 * sub_08017BD4 @ 0x08017BD4, sub_08017BFC @ 0x08017BFC, sub_08017C24 @ 0x08017C24
 */

bool8 EventOp_CopyByte(s16 a)
{
    *(u8 *)gUnknown_0200C528[a].unk04->unk04 = *(u8 *)gUnknown_0200C528[a].unk04->unk0c;
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08017BD4\n.thumb_set sub_08017BD4, EventOp_CopyByte\n");

bool8 EventOp_CopyHalfword(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    *(u16 *)p->unk04 = *(u16 *)p->unk0c;
    gUnknown_0200C528[a].unk04 = p + 1;
    return TRUE;
}
asm(".global sub_08017BFC\n.thumb_set sub_08017BFC, EventOp_CopyHalfword\n");

bool8 EventOp_CopyWord(s16 a)
{
    struct Unk0200C528Node *p = gUnknown_0200C528[a].unk04;

    *(u32 *)p->unk04 = *(u32 *)p->unk0c;
    gUnknown_0200C528[a].unk04 = p + 1;
    return TRUE;
}
asm(".global sub_08017C24\n.thumb_set sub_08017C24, EventOp_CopyWord\n");
