#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032134.
 * sub_08032134 @ 0x08032134
 */

#include "proc.h"
#include "hardware.h"

void LinkLobby_ResetIfConnectionLost(ProcPtr proc)
{
    if (gUnknown_0849B018->unk04 == 7)
    {
        gUnknown_0849B018->unk01 = 0;
        gUnknown_0849B018->unk04 = 0;
        gUnknown_0849B018->unk06 = -1;
        gUnknown_0849B018->unk07 = 0;
        gUnknown_0849B018->unk08 = 0;
        gUnknown_0849B018->unk09 = 0;
        gUnknown_0849B018->unk1c = 0;
        gUnknown_0849B018->unk1d = 0;
        gUnknown_0849B018->unk1e = 0;

        LinkResetKeySync();
        SioResetBuffers();
        LinkClearMapListAndNames();

        REG_RCNT = 0x8000;
        Proc_Goto(proc, 2);
    }
}
asm(".global sub_08032134\n.thumb_set sub_08032134, LinkLobby_ResetIfConnectionLost\n");
