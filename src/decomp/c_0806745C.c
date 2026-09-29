#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806745C.
 * sub_0806745C @ 0x0806745C, sub_08067480 @ 0x08067480
 */

#include "hardware.h"
#include "proc.h"
struct Unk6745CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};
struct Unk67480Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* The count-UP twin of BlendFromWhite_Loop: same publish-then-test-then-step shape
 * against the same volatile blend coefficient, running 0 -> 0x1f instead of
 * down to zero. See BlendFromWhite_Loop for the derivation of the reused `cmp`
 * operand. */
void BlendToWhite_Loop(struct Unk6745CProc *proc)
{
    gUnknown_03001FFC = proc->unk58;

    if (proc->unk58 > 0x1e)
        Proc_Break(proc);

    proc->unk58++;
}
asm(".global sub_0806745C\n.thumb_set sub_0806745C, BlendToWhite_Loop\n");

/* Arms a 0x20-frame counter and kicks off a fade. The store puts the value in
 * r1 and keeps the proc in r0 only because r0 is about to become the call's
 * argument register; -1 then materialises as `movs r0,#1; rsbs r0,r0,#0`,
 * which is how THUMB spells a small negative constant. */
void PaletteFadeFromWhite_Init(struct Unk67480Proc *proc)
{
    proc->unk58 = 0x20;
    ColFadeFromWhite(-1);
    EnablePaletteSync();
}
asm(".global sub_08067480\n.thumb_set sub_08067480, PaletteFadeFromWhite_Init\n");
