#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027414.
 * sub_08027414 @ 0x08027414
 */

#include "proc.h"
/* Family F025 with one extra leading call. DecrementMapLock is `void (void)`, so
 * the untouched r0 at that `bl` carries nothing -- the proc is saved in r4
 * only because it is needed after. */
struct Unk27414Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x50);
    /* 50 */ void *unk50;
};

void sub_08027414(struct Unk27414Proc *proc)
{
    DecrementMapLock();
    AP_Delete(proc->unk50);
}
