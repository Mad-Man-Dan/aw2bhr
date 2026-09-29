#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032134.
 * sub_08032134 @ 0x08032134
 */

#include "proc.h"
#include "hardware.h"

void sub_08032134(ProcPtr proc)
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

        sub_0802F23C();
        SioResetBuffers();
        sub_0802F28C();

        REG_RCNT = 0x8000;
        Proc_Goto(proc, 2);
    }
}
