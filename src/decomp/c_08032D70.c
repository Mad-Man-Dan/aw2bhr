#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032D70.
 * sub_08032D70 @ 0x08032D70
 */

#include "proc.h"
struct Unk32D70Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ STRUCT_PAD(0x2a, 0x2c);
    /* 0x2c */ int unk2c;
};
struct Unk32D70Target
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

/* The single `movs r1, #1` serves both stores: agbcc reuses a register already
 * known to hold the constant rather than reloading it, so the literal 1 is
 * written twice in the source and once in the output. */

void LinkConfirmDialog_Init(struct Unk32D70Proc *proc)
{
    Decompress(gUnknown_081D3C34, (void *)0x06015780);

    ((struct Unk32D70Target *)Proc_Find(gUnknown_0849B6B0))->unk58 = 1;

    proc->unk2c = 0;
    proc->unk29 = 1;
}
asm(".global sub_08032D70\n.thumb_set sub_08032D70, LinkConfirmDialog_Init\n");
