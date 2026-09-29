#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067498.
 * PaletteFade_Loop @ 0x08067498
 */

#include "proc.h"
struct Unk67498Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* MATCHED, and the first of a byte-identical pair with sub_080674D0. A
 * countdown proc tick: run two subsystem updates, decrement the counter and
 * break when it goes negative.
 *
 * `sub_080718F0` is one of the ARM interworking thunks in
 * data/asm-resident.json (`bx pc; nop` into an ARM `b`), and calling it needs
 * nothing special -- it is an ordinary `bl` from C. */
void PaletteFade_Loop(struct Unk67498Proc *proc)
{
    sub_080718F0();
    EnablePaletteSync();

    proc->unk58--;

    if (proc->unk58 < 0)
        Proc_Break(proc);
}
asm(".global sub_08067498\n.thumb_set sub_08067498, PaletteFade_Loop\n");
