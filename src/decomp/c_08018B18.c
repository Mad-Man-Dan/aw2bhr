#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018B18.
 * sub_08018B18 @ 0x08018B18, sub_08018B40 @ 0x08018B40
 */

bool8 EventOp_SetCoScreenDrawHook(s16 a)
{
    gUnknown_03002F20 = (void (*)(void))gUnknown_0200C528[a].unk04->unk04;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08018B18\n.thumb_set sub_08018B18, EventOp_SetCoScreenDrawHook\n");

bool8 EventOp_InstallCallback(s16 a)
{
    struct Unk0200C528Node *p;

    gUnknown_0200C528[a].unk08 = (p = gUnknown_0200C528[a].unk04)->unk04;
    gUnknown_0200C528[a].unk04 = p + 1;
    return FALSE;
}
asm(".global sub_08018B40\n.thumb_set sub_08018B40, EventOp_InstallCallback\n");
