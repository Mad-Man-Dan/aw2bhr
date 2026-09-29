#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E4BC.
 * sub_0806E4BC @ 0x0806E4BC
 */

#include "proc.h"
/* SEVEN parameters -- four in registers, three at [sp,#0x18]/[sp,#0x1c]/
 * [sp,#0x20] over a 24-byte prologue -- and the seventh is Proc_Start's parent,
 * forwarded in r1 without ever being copied. The fourth is the only narrow one
 * (`lsls #0x10; lsrs #0x10`, PROMOTE_MODE) and it is the +0x40 halfword.
 *
 * It RETURNS the proc: the epilogue is `pop {r1}; bx r1` with r0 still holding
 * Proc_Start's result, which a void function would have popped into r0.
 *
 * `p->unk3c = p->unk3e = 0x100` and not two statements -- one `movs`/`lsls`
 * pair feeds two `strh`s and the +0x3e store comes first, i.e. it is the inner
 * assignment. */
struct Unk08582B14Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ u16 unk3c;
    /* 0x3e */ u16 unk3e;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u8 filler_42[0x02];
    /* 0x44 */ int unk44;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 filler_49[0x03];
    /* 0x4c */ int unk4c;
};

ProcPtr StartSoundRoomButton(int a1, int a2, int a3, u16 a4, int a5, int a6, ProcPtr parent)
{
    struct Unk08582B14Proc *p;

    p = Proc_Start(gUnknown_08582B14, parent);

    p->unk2c = a1;
    p->unk30 = a2;
    p->unk34 = a3;
    p->unk40 = a4;
    p->unk38 = a5;
    p->unk3c = p->unk3e = 0x100;
    p->unk44 = a6;
    p->unk48 = 0;
    p->unk4c = 0;

    return p;
}
asm(".global sub_0806E4BC\n.thumb_set sub_0806E4BC, StartSoundRoomButton\n");
