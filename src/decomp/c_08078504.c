#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078504.
 * sub_08078504 @ 0x08078504
 */

#include "proc.h"
/* Same head as sub_08078498 -- clear gUnknown_03002F08.unk00, hand the proc to
 * sub_08014BC0 -- and then install the script stashed at +0x54 by
 * src/decomp/c_08078540.c's starter. That starter types +0x54 as `void *`
 * because it only stores it; here it is dereferenced by StartEventScript's
 * `const u8 *` parameter, which is what pins the field's type. StartEventScript's
 * result is discarded, as in the other wrappers that call it. */

struct UnkProc8615AAC
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ const u8 *unk_54;
};

void sub_08078504(struct UnkProc8615AAC *proc)
{
    gUnknown_03002F08.unk00 = 0;
    sub_08014BC0(proc);
    StartEventScript(proc->unk_54);
}
