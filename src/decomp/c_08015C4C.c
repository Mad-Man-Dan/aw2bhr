#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015C4C.
 * sub_08015C4C @ 0x08015C4C, sub_08015C64 @ 0x08015C64, sub_08015C7C @ 0x08015C7C, sub_08015C94 @ 0x08015C94, sub_08015CB4 @ 0x08015CB4, sub_08015CCC @ 0x08015CCC
 */

void SetSlotScriptEndCallback(u8 a, u32 b)
{
    gUnknown_03001470[a].unk0c = b;
}
asm(".global sub_08015C4C\n.thumb_set sub_08015C4C, SetSlotScriptEndCallback\n");

void SetSlotScriptCallback(u8 a, u32 b)
{
    gUnknown_03001470[a].unk08 = b;
}
asm(".global sub_08015C64\n.thumb_set sub_08015C64, SetSlotScriptCallback\n");

u32 GetSlotScriptCallback(u8 a)
{
    return gUnknown_03001470[a].unk08;
}
asm(".global sub_08015C7C\n.thumb_set sub_08015C7C, GetSlotScriptCallback\n");

void SetSlotScriptCursor(u8 a, const void *b)
{
    gUnknown_03001470[a].unk04 = b;
    gUnknown_03001470[a].unk10 = 0;
}
asm(".global sub_08015C94\n.thumb_set sub_08015C94, SetSlotScriptCursor\n");

void SetSlotScriptOrder(u8 a, u8 b)
{
    gUnknown_03001470[a].unk14 = b;
}
asm(".global sub_08015CB4\n.thumb_set sub_08015CB4, SetSlotScriptOrder\n");

u8 GetSlotScriptOrder(u8 a)
{
    return gUnknown_03001470[a].unk14;
}
asm(".global sub_08015CCC\n.thumb_set sub_08015CCC, GetSlotScriptOrder\n");
