#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080114A0.
 * sub_080114A0 @ 0x080114A0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
struct Unk80114A0Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

/* Both gUnknown_03002F3C and gUnknown_03001408 are VOLATILE, and this function
 * is what proves it -- twice each. The `+=` result is re-read from memory for
 * the clamp instead of being reused; and every read is `ldrh` followed by
 * `lsls #0x10; asrs #0x10` rather than the single `ldrsh` a plain s16 folds to,
 * which is the same tell recorded on gUnknown_03002F30. Declared non-volatile
 * the body is 144 bytes against 156.
 *
 * The two Interpolate arms are written out in full: only the first argument
 * differs, and agbcc's cross-jumping merges the tail from `movs r2,#0` onward
 * by itself. A ternary in the argument computes the flag first and shares the
 * setup, which is a different instruction stream. */
void WipeFromBlack_Loop(struct Unk80114A0Proc *proc)
{
    gUnknown_03002F3C += proc->unk64;

    if (gUnknown_03002F3C > 0x13F)
        gUnknown_03002F3C = 0x140;

    if (proc->unk64 <= 8)
        gUnknown_03001408 = Interpolate(0, 0x140, 0, gUnknown_03002F3C, 0x140);
    else
        gUnknown_03001408 = Interpolate(1, 0x140, 0, gUnknown_03002F3C, 0x140);

    if (gUnknown_03001408 <= 0)
    {
        gUnknown_03001408 = 0;
        gDispIo.disp_ct.win0_enable = 0;
        Proc_Break(proc);
    }
}

asm(".global sub_080114A0\n.thumb_set sub_080114A0, WipeFromBlack_Loop\n");
