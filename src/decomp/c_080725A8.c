#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080725A8.
 * sub_080725A8 @ 0x080725A8, sub_080725E4 @ 0x080725E4, sub_080725FC @ 0x080725FC
 */

#include "proc.h"
struct Unk725A8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ s16 unk30;
    /* 32 */ s16 unk32;
    /* 34 */ s16 unk34;
    /* 36 */ s16 unk36;
    /* 38 */ s16 unk38;
    /* 3a */ s16 unk3a;
};
struct Unk725E4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x3a);
    /* 3a */ s16 unk3a;
};
struct Unk725FCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x3a);
    /* 3a */ s16 unk3a;
};

/* The gUnknown_08613F2C starter behind StartPaletteAnimatorReverse and StartPaletteAnimatorNormal. Five
 * parameters -- the fifth arrives on the stack at [sp,#0x14], which is exactly
 * the 0x14 bytes this prologue pushes, and it is the parent.
 *
 * `lsrs r1,r4,#0x1f; adds r4,r4,r1; asrs r4,r4,#1` is agbcc's SIGNED divide by
 * two (bias by the sign bit, then arithmetic shift), so `c / 2` and not
 * `c >> 1`; that also pins parameter 3 as signed.
 *
 * +0x36 is written before +0x34 -- the ROM's order, not a transposition -- and
 * the returned proc is settled from the callers, both of which write +0x3a
 * through the result. */
ProcPtr StartPaletteAnimatorExt(int a, int b, int c, int d, ProcPtr parent)
{
    struct Unk725A8Proc *proc = Proc_Start(gUnknown_08613F2C, parent);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk32 = c / 2;
    proc->unk36 = d;
    proc->unk34 = d;
    proc->unk38 = 0;
    proc->unk3a = 0;

    return proc;
}
asm(".global sub_080725A8\n.thumb_set sub_080725A8, StartPaletteAnimatorExt\n");

/* Mode-0 wrapper around StartPaletteAnimatorExt: forwards all five arguments unchanged
 * (the fifth is reloaded from the incoming stack slot and restacked) and then
 * writes the mode flag. StartPaletteAnimatorExt already zeroes +0x3a, so this store is
 * redundant at runtime -- which is exactly why the pair reads as generated
 * "mode 0 / mode 1" wrappers rather than one function with a parameter. */
void StartPaletteAnimatorReverse(int a, int b, int c, int d, ProcPtr parent)
{
    struct Unk725E4Proc *proc = StartPaletteAnimatorExt(a, b, c, d, parent);

    proc->unk3a = 0;
}
asm(".global sub_080725E4\n.thumb_set sub_080725E4, StartPaletteAnimatorReverse\n");

/* The mode-1 half of the StartPaletteAnimatorReverse pair; see the note there. */
void StartPaletteAnimatorNormal(int a, int b, int c, int d, ProcPtr parent)
{
    struct Unk725FCProc *proc = StartPaletteAnimatorExt(a, b, c, d, parent);

    proc->unk3a = 1;
}
asm(".global sub_080725FC\n.thumb_set sub_080725FC, StartPaletteAnimatorNormal\n");
