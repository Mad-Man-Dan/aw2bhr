#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031BF0.
 * sub_08031BF0 @ 0x08031BF0, sub_08031C1C @ 0x08031C1C
 */

#include "proc.h"

/* The (u32) cast is StartSioBigReceive's own promoted signature
 * (src/decomp/c_080337D8.c), not a spelling choice; gUnknown_02000000 is a
 * u8 array and the address goes out unchanged either way. */

void LinkReceiveMapData(ProcPtr parent)
{
    StartSioBigReceive((u32)gUnknown_02000000, 0, parent);
    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 10, 2);
}
asm(".global sub_08031BF0\n.thumb_set sub_08031BF0, LinkReceiveMapData\n");

/* LinkReceiveMapData's five-argument twin: the same gUnknown_02000000 buffer and the
 * same LinkScreenSetMessage round-trip afterwards, differing only in the extra
 * 0xA5C size, the two zeros, and the message id. */

void LinkSendMapData(ProcPtr parent)
{
    StartSioBigSend((u32)gUnknown_02000000, 0xa5c, 0, 0, parent);
    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 9, 2);
}
asm(".global sub_08031C1C\n.thumb_set sub_08031C1C, LinkSendMapData\n");
