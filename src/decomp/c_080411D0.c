#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080411D0.
 * sub_080411D0 @ 0x080411D0
 */

#include "proc.h"
/* The parameter is not a Proc -- it is only ever the parent handed through to
 * Proc_StartBlocking and the source of two bytes. Modelled as filler for the
 * same reason c_08041180.c models its own: nothing here proves a proc layout.
 */
struct Unk411D0Src
{
    /* 00 */ u8 filler_00[0x48];
    /* 48 */ u8 unk48;
    /* 49 */ u8 unk49;
};
struct Unk411D0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ u16 unk4c;
    /* 4e */ u16 unk4e;
};

/* A proc starter that forwards its own only parameter as the parent, then
 * widens two of that parameter's bytes into halfwords on the new proc. All
 * four `adds rN, #imm` here are THUMB displacement limits (ldrb 0-31,
 * strh 0-62 even), not addresses being taken.
 */
void CaptureAnim_StartCountUp(struct Unk411D0Src *a)
{
    struct Unk411D0Proc *proc = Proc_StartBlocking(gUnknown_0849FE34, a);

    proc->unk4c = a->unk48;
    proc->unk4e = a->unk49;
}
asm(".global sub_080411D0\n.thumb_set sub_080411D0, CaptureAnim_StartCountUp\n");
