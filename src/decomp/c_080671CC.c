#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080671CC.
 * sub_080671CC @ 0x080671CC
 */

#include "hardware.h"
#include "proc.h"
struct Unk671CCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

/* A countdown proc tick that publishes its counter as the blend coefficient.
 * The `strh` of a word-sized field into `volatile u16 gUnknown_03001FFC` is a
 * plain truncating store, so it costs no shift pair -- the store-forwarding
 * case in docs/agbcc-codegen.md needs the global to be READ back, and this
 * function never reads it.
 *
 * The `cmp r0, #0` reuses the value already in r0 rather than reloading +0x58:
 * a volatile HImode store to a different object does not invalidate the SImode
 * load. The reload after the `bl` is Proc_Break clobbering memory. */
void BlendFromWhite_Loop(struct Unk671CCProc *proc)
{
    gUnknown_03001FFC = proc->unk58;

    if (proc->unk58 == 0)
        Proc_Break(proc);

    proc->unk58--;
}
asm(".global sub_080671CC\n.thumb_set sub_080671CC, BlendFromWhite_Loop\n");
