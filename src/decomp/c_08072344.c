#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072344.
 * sub_08072344 @ 0x08072344
 */

#include "hardware.h"
#include "proc.h"
struct Unk72344Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
};

/* One step of the gUnknown_08613EE4 fade: advance both accumulators by the
 * per-frame delta StartFadeCore parked at +0x54 and report whether the fade is
 * still running. Returns 1 while it is and on the final frame (which also
 * blanks the backdrop), 0 only once the second accumulator has run past
 * 0x1ff. */
u8 FadeCore_Tick(ProcPtr procPtr)
{
    struct Unk72344Proc *proc = procPtr;

    proc->unk58 += proc->unk54;
    proc->unk5c += proc->unk54;

    if (proc->unk58 > 0xf)
        proc->unk58 -= 0x10;
    else if (proc->unk5c != proc->unk54)
        return 1;

    if (proc->unk5c > 0x1ff)
        return 0;

    sub_080718F0();
    gPal[0] = 0;
    EnablePaletteSync();

    return 1;
}
asm(".global sub_08072344\n.thumb_set sub_08072344, FadeCore_Tick\n");
