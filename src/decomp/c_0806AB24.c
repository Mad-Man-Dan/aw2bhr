#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806AB24.
 * sub_0806AB24 @ 0x0806AB24
 */

/* Lays out a multi-line message box: sub_0807B7BC wraps the string
 * GetMapName returned into `proc->unk2f[]`, returning the LINE COUNT and
 * writing the last line's residual width back through its `u16 *`
 * out-parameter. The first line is then centred against the 0xf0 screen width
 * and every later line's start is accumulated from the one before it.
 *
 * `asrs r0, r0, #1` with no round-toward-zero correction is `>> 1` on a signed
 * int, not `/ 2`.
 *
 * `rest` is the FIRST operand of the sum. agbcc expands a PLUS_EXPR's operands
 * left to right, and expanding a MEM emits only its ADDRESS -- so writing
 * `rest + w * (n - 1)` puts `add r2, sp, #4` ahead of the `subs r0, #1` while
 * leaving the `ldrh` after the multiply, which is exactly what the ROM shows.
 * `w * (n - 1)` and not `(n - 1) * w`: the operand copied into the MUL's
 * destination is the left one.
 *
 * `w` has to be a local. The accumulate loop stores through `proc->unk2f`,
 * which may alias the ROM table's own load, so loop.c cannot hoist a
 * `gUnknown_085816B4[n]` written inside the loop -- yet the ROM keeps it in r3
 * across all of it. */

struct Unk806AB24
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u8 filler_2c[0x03];
    /* 0x2f */ u8 unk2f[0x15];
    /* 0x44 */ u16 unk44;
    /* 0x46 */ u8 filler_46[0x02];
    /* 0x48 */ int unk48;
    /* 0x4c */ u8 filler_4c[0x06];
    /* 0x52 */ u16 unk52;
};

void CreditsMissionLine_LayoutMapName(struct Unk806AB24 *proc)
{
    u16 rest;
    u16 w;
    int n;
    int i;

    n = sub_0807B7BC(GetMapName(proc->unk2a), &rest, proc->unk2f, proc->unk52, proc);
    proc->unk48 = n;
    w = gUnknown_085816B4[n];
    proc->unk2f[0] = (0xf0 - (rest + w * (n - 1))) >> 1;

    for (i = 1; i < proc->unk48; i++)
        proc->unk2f[i] += proc->unk2f[i - 1] + w;

    proc->unk44 = 0x140;
}
asm(".global sub_0806AB24\n.thumb_set sub_0806AB24, CreditsMissionLine_LayoutMapName\n");
