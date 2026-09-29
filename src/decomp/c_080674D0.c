#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080674D0.
 * sub_080674D0 @ 0x080674D0
 */

#include "proc.h"
struct Unk674D0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* MATCHED. Byte-for-byte the same function as PaletteFade_Loop -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void sub_080674D0(struct Unk674D0Proc *proc)
{
    sub_080718F0();
    EnablePaletteSync();

    proc->unk58--;

    if (proc->unk58 < 0)
        Proc_Break(proc);
}
