#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806778C.
 * sub_0806778C @ 0x0806778C
 */

#include "proc.h"
struct Unk6778CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
};

/* A scroll tick gated on +0x5c: step two positions by their per-frame deltas
 * and republish them. All three arguments narrow with `lsl #16; lsr #16`
 * because SetBgScrollShadow takes (u16, u16, u16) -- the fields themselves are
 * plain words, and the updated values stay live in r1/r2 across their own
 * `str`s, since a word store does not invalidate the SImode value. */
void IntroBgScroll_Loop(struct Unk6778CProc *proc)
{
    if (proc->unk5c != 0)
    {
        proc->unk2c += proc->unk34;
        proc->unk30 += proc->unk38;
        SetBgScrollShadow(proc->unk58, proc->unk2c, proc->unk30);
    }
}
asm(".global sub_0806778C\n.thumb_set sub_0806778C, IntroBgScroll_Loop\n");
