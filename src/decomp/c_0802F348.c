#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F348.
 * sub_0802F348 @ 0x0802F348
 */

void LinkResetState(void)
{
    gPlaySt.savingEnabled = 0;
    gUnknown_0849B018->unk00 = 0;
    gUnknown_0849B018->unk01 = 0;
    gUnknown_0849B018->unk04 = 0;
    gUnknown_0849B018->unk02 = 0;
    gUnknown_030044D8 = 0;
    gUnknown_0849B018->unk06 = 0xff;
    gUnknown_0849B018->unk07 = 0;
    gUnknown_0849B018->unk08 = 0;
    gUnknown_0849B018->unk09 = 0;
    gUnknown_0849B018->unk1c = 0;
    gUnknown_0849B018->unk1d = 0;
    gUnknown_0849B018->unk1e = 0;
    LinkResetKeySync();
    SioResetBuffers();
    LinkClearMapListAndNames();
    gUnknown_0849B018->unk1ab0 = 0;
    gUnknown_0849B018->unk1ab2 = 0;
    gUnknown_0849B018->unk1ab3 = 0;
    gUnknown_0849B018->unk1ab4 = -1;
    gUnknown_030040C0 = gUnknown_08090C38;
    gUnknown_0300477C = 0;
}
asm(".global sub_0802F348\n.thumb_set sub_0802F348, LinkResetState\n");
