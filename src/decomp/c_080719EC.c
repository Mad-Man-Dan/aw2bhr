#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080719EC.
 * sub_080719EC @ 0x080719EC
 */

#include "proc.h"
#include "hardware.h"
struct Unk80719EC
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x04];
    /* 0x64 */ s16 unk64;
};

void DigitPicker_Loop(struct Unk80719EC *proc)
{
    if (proc->unk64 != GetMapLock())
        return;

    PutDecimalNumberSprites(0x78, 0x50, proc->unk58);
    PutDecimalNumberSprites(0x78 - proc->unk5c * 8, 0x58, 0);

    if (gpKeySt->repeated & DPAD_LEFT)
    {
        if (proc->unk5c <= 3)
            proc->unk5c++;
    }

    if (gpKeySt->repeated & DPAD_RIGHT)
    {
        if (proc->unk5c > 0)
            proc->unk5c--;
    }

    if (gpKeySt->repeated & DPAD_UP)
    {
        proc->unk58 += gUnknown_08613E48[proc->unk5c];

        if (proc->unk58 > 0x1869f)
            proc->unk58 = 0x1869f;
    }

    if (gpKeySt->repeated & DPAD_DOWN)
    {
        proc->unk58 -= gUnknown_08613E48[proc->unk5c];

        if (proc->unk58 < 0)
            proc->unk58 = 0;
    }

    if (gpKeySt->pressed & 1)
    {
        proc->unk64 = GetMapLock();
        Proc_Break(proc);
    }

    if (gpKeySt->pressed & 2)
        Proc_End(proc);
}
asm(".global sub_080719EC\n.thumb_set sub_080719EC, DigitPicker_Loop\n");
