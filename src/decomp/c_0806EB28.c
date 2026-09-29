#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806EB28.
 * sub_0806EB28 @ 0x0806EB28
 */

#include "proc.h"
/* Picks the next of four palettes, skipping whichever one the proc already has,
 * and applies it. gUnknown_08582754 is an array of POINTERS, not palette data:
 * the `lsls #2` index is followed by a LOAD through the result.
 *
 * The `if` has no else and its body FALLS THROUGH between the `bne` and the
 * label, which is the ordinary guard shape -- unlike the joining `if/else`es
 * elsewhere in this block, there is nothing here for an out-of-line arm to be.
 *
 * The parameter must stay `ProcPtr` because include/unknown-functions.h already
 * declares it that way and both callers in src/decomp/c_0806EB5C.c pass
 * differently-typed procs; the cast to a layout is free. */
struct Unk6EB28Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x36);
    /* 36 */ u16 unk36;
};

void SoundRoomCycleBgPalette(ProcPtr procArg)
{
    struct Unk6EB28Proc *proc = procArg;
    int index = gGameClock & 3;

    if (proc->unk36 == index)
        index++;

    proc->unk36 = index;
    ApplyPaletteExt(gUnknown_08582754[index], 0x20, 0x20);
}
asm(".global sub_0806EB28\n.thumb_set sub_0806EB28, SoundRoomCycleBgPalette\n");
