#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074648.
 * sub_08074648 @ 0x08074648
 */

#include "hardware.h"
#include "proc.h"
/* DarkenRampDown_Loop's count-up twin: same publish-then-step-then-test shape, +1 and
 * a 0xf ceiling instead of -1 and a sign test.
 */
struct Unk8074628
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk_4c;
};

void DarkenRampUp_Loop(struct Unk8074628 *proc)
{
    gUnknown_03001FFC = proc->unk_4c;
    proc->unk_4c++;

    if ((s16)proc->unk_4c > 0xf)
        Proc_Break(proc);
}
asm(".global sub_08074648\n.thumb_set sub_08074648, DarkenRampUp_Loop\n");
