#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018E7C.
 * sub_08018E7C @ 0x08018E7C, sub_08018EB0 @ 0x08018EB0
 */

bool8 EventOp_EndCursorScript(s16 a)
{
    EndEventScript(gUnknown_0848A378);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018E7C\n.thumb_set sub_08018E7C, EventOp_EndCursorScript\n");

bool8 sub_08018EB0(s16 a)
{
    if (sub_0802C550() == 0)
        gUnknown_0200C528[a].unk04++;
    return FALSE;
}
