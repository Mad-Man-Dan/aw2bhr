#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071D70.
 * sub_08071D70 @ 0x08071D70
 */

#include "hardware.h"
#include "proc.h"
/* The fade-UP stepper: accumulate +0x64 into the +0x66 fixed-point
 * accumulator, clamp at 0x100, publish the top four bits as BLDY. Ends itself
 * once BLDY has reached 0x10. FadeFromCommon_OnLoopUnused is the matching fade-DOWN.
 *
 * `+=` and not `unk66 = unk64 + unk66`: the two spell DIFFERENT code. The
 * compound assignment computes the destination address first (+0x66 into the
 * low register, +0x64 second), which is the ROM's order; writing the addition
 * out longhand computes +0x64 first and swaps the pointer registers. It costs
 * nothing in size, so a size check would miss it.
 *
 * unk66 is SIGNED -- the clamp compares it `lsls #0x10; asrs #0x10` against
 * 0xff, and the publish is an arithmetic `asrs #0x14`. The threshold fits an
 * imm8 here, so the comparison is done on the sign-extended value; its 0x08011
 * twin sub_080110A4 needs 0xFFF, which does not fit, and agbcc compares in the
 * shifted domain against a pooled 0x0FFF0000 instead. Same source shape, two
 * different-looking comparisons. */
struct Unk8071D70
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

void FadeToCommon_OnLoopUnused(struct Unk8071D70 *proc)
{
    if (gUnknown_03001FFC == 0x10)
    {
        Proc_End(proc);
        return;
    }

    proc->unk66 += proc->unk64;

    if (proc->unk66 > 0xFF)
        proc->unk66 = 0x100;

    gUnknown_03001FFC = proc->unk66 >> 4;
}
asm(".global sub_08071D70\n.thumb_set sub_08071D70, FadeToCommon_OnLoopUnused\n");
