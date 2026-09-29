#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033638.
 * sub_08033638 @ 0x08033638, sub_08033678 @ 0x08033678
 */

#include "proc.h"
/* Polls the link queue and, when a packet arrives, unpacks four bytes of it
 * into the proc and breaks the proc. The `!= -1` test re-narrows
 * LinkReceiveCommand's `s16` return with `lsls #0x10; asrs #0x10`, which is that
 * declared width and not a cast here.
 *
 * The halfword goes through an `int` TEMPORARY, and that is load-bearing:
 * written inline as `proc->unk2a = (b[1] << 8) | b[2]`, agbcc puts the shifted
 * value in r1 and the second byte in r0, where the ROM has them the other way
 * round. Naming the value gives the first operand the result register and
 * matches. The two spellings are otherwise identical, so this is register
 * allocation reacting to a declared local, not a difference in the expression. */
struct Unk33678Proc
{
    /* 00 */ u8 filler_00[0x28];
    /* 28 */ u8 unk28;
    /* 29 */ u8 filler_29[0x01];
    /* 2a */ u16 unk2a;
    /* 2c */ u8 filler_2c[0x02];
    /* 2e */ u8 unk2e;
};

/* Clears the link state: the gate byte, then two groups of VOLATILE halfwords
 * in struct Unk0849B018, then the big per-army reset.
 *
 * Every store here is preceded by a dead load of the address being stored to.
 * That is the volatile + HImode-aggregate-member signature this tree has
 * already recorded for struct Unk0849B01C, and SioResetBuffers -- still asm --
 * shows the identical pattern at these same six offsets, so the qualifier is
 * a property of the object and not of this function.
 *
 * The record pointer is bound to a NAMED local. Left as `gUnknown_0849B018->`
 * at each use the body is identical, but the two pool words come out in the
 * other order and the gate store is emitted before the pointer load. */
void SioBigReceive_Init(void)
{
    struct Unk0849B018 *p = gUnknown_0849B018;

    gUnknown_030044D8 = 0;

    p->unk22 = p->unk20 = 0;
    p->unk24[0] = p->unk24[1] = p->unk24[2] = p->unk24[3] = 0;

    SioResetBuffers();
}
asm(".global sub_08033638\n.thumb_set sub_08033638, SioBigReceive_Init\n");

void SioBigReceive_ReadHeader(struct Unk33678Proc *proc)
{
    int v;

    if (LinkReceiveCommand((void *)gUnknown_03004400, 0) != -1)
    {
        proc->unk28 = gUnknown_03004400[0];
        v = (gUnknown_03004400[1] << 8) | gUnknown_03004400[2];
        proc->unk2a = v;
        proc->unk2e = gUnknown_03004400[3];
        Proc_Break(proc);
    }
}
asm(".global sub_08033678\n.thumb_set sub_08033678, SioBigReceive_ReadHeader\n");
