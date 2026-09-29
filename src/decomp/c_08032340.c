#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032340.
 * sub_08032340 @ 0x08032340
 */

#include "proc.h"
struct Unk32340Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void LinkPlayerCursor_Draw(ProcPtr procArg, s16 a, s16 b)
{
    struct Unk32340Proc *proc = procArg;
    s16 v = a;
    int y;
    int x;

    if (a != b)
        v = a + ((b - a) >> 1);

    y = v;
    x = gUnknown_0849B108[proc->unk58 * 2] + 0x48;

    DrawOamObject(0x50, y + x,
        gUnknown_0849B108[proc->unk58 * 2 + 1] + 0x58,
        0, 0);

    DrawOamObject(0x50,
        (y - gUnknown_0849B108[proc->unk58 * 2] + 0x78) | 0x3000,
        0x3c - gUnknown_0849B108[proc->unk58 * 2 + 1],
        0, 0);

    proc->unk58 = (proc->unk58 + 1) & 0x1f;
}
asm(".global sub_08032340\n.thumb_set sub_08032340, LinkPlayerCursor_Draw\n");
