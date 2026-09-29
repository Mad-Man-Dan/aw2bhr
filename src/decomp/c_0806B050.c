#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B050.
 * sub_0806B050 @ 0x0806B050
 */

#include "proc.h"
#include "hardware.h"
struct Unk6B050Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

/* The countdown member of the same blend family. The decrement is OUTSIDE the
 * `if` -- the ROM reloads +0x58 after the Proc_Break call rather than reusing
 * the register the compare left it in, which is what a statement following the
 * conditional looks like. One `movs r3, #0` feeds both zero stores. */
void CreditsListFade_Loop(struct Unk6B050Proc *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0xE - proc->unk58;
    gUnknown_03001FFC = 0;

    if (proc->unk58 == 0)
        Proc_Break(proc);

    proc->unk58--;
}
asm(".global sub_0806B050\n.thumb_set sub_0806B050, CreditsListFade_Loop\n");
