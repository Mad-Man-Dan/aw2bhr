#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C95C.
 * sub_0806C95C @ 0x0806C95C
 */

#include "hardware.h"
#include "proc.h"
/* The slide-BACK-in twin of src/decomp/c_0806C8DC.c's CreditsPageWipe_OutLoop: same three
 * scroll shadows, the same `-a` reused for both window edges, the same
 * counter/Proc_Break tail -- but 16 frames instead of 20, mode 0 instead of 1,
 * and the endpoints swapped so the offset runs -0xb0 -> 0. There is no
 * SetCreditsWindow1Rect call and no window teardown on the last frame.
 *
 * NOTE: this is the LAST function in asm/code-0801D390.s, so its size comes
 * from awlib.estimate_size rather than a next-symbol delta. That estimator
 * counted its three `_0806C9xx: .4byte sym` pool words as 2-byte instructions
 * and indexed the function at 82 bytes instead of 88; the fix is in
 * tools/awlib.py (wave 32, W32-C). */
struct Unk0806C95CProc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CreditsPageWipe_InLoop(struct Unk0806C95CProc *proc)
{
    int a;

    a = Interpolate(0, -0xb0, 0, proc->unk58, 0x10);

    gUnknown_03001FF8 = a;
    gUnknown_03002B34 = a;
    gUnknown_030030A0 = a;

    SetCreditsWindow0Rect(-a, 0, -a + 0x40, 0xa0);

    if (proc->unk58 > 0xf)
        Proc_Break(proc);
    else
        proc->unk58++;
}
asm(".global sub_0806C95C\n.thumb_set sub_0806C95C, CreditsPageWipe_InLoop\n");
