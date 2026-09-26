#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032D38.
 * sub_08032D38 @ 0x08032D38
 */

struct Unk32D38Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void sub_08032D38(struct Unk32D38Proc *proc)
{
    if (proc->unk58 == 0)
    {
        sub_08032A00();
        sub_08032950();
    }
}
