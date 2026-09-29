#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067948.
 * sub_08067948 @ 0x08067948, sub_080679D8 @ 0x080679D8
 */

#include "hardware.h"
#include "proc.h"
/* The animated twin of IntroBgZoom_Loop: two Interpolates per frame drive the BG2
 * affine matrix's rotation and scale, and the proc breaks on the last frame.
 *
 * The `if` picks Interpolate's MODE only -- the other four arguments are
 * written out in both arms and agbcc cross-jumps everything from the `bl`
 * backwards, which is why the ROM shows the two argument blocks in full and a
 * shared call.
 *
 * The 0x18-byte frame is `struct BgAffineSrcData` at sp+4 with the outgoing
 * fifth argument at sp+0 -- the same layout src/decomp/c_080678EC.c records,
 * one word lower. */
struct Unk08067948Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3c */ s32 unk3c;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4c */ s32 unk4c;
    /* 0x50 */ u8 filler_50[0x04];
    /* 0x54 */ s32 unk54;
};
/* TEN parameters -- four in registers and six at [sp,#0x18] through
 * [sp,#0x2c] over a 24-byte prologue -- and the tenth is Proc_Start's parent,
 * loaded straight into r1 and never copied. The first is stored LAST (at
 * +0x4c), which is why it is the one that needs a high register.
 *
 * Only the second is transformed: `<< 4` into +0x2c, a 16-unit fixed point.
 * The two trailing zeroes are one materialised 0 shared between the +0x54 word
 * and the +0x50 byte. */
struct Unk08581014Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ s32 unk2c;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3c */ s32 unk3c;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4c */ s32 unk4c;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 filler_51[0x03];
    /* 0x54 */ s32 unk54;
};

void IntroBgAffineTween_Loop(struct Unk08067948Proc *proc)
{
    struct BgAffineSrcData src;
    int a;
    int b;

    if (proc->unk4c != 0)
        a = Interpolate(4, proc->unk38, proc->unk3c, proc->unk54, proc->unk48);
    else
        a = Interpolate(1, proc->unk38, proc->unk3c, proc->unk54, proc->unk48);

    b = Interpolate(0, proc->unk40, proc->unk44, proc->unk54, proc->unk48);

    src.texX = proc->unk30 << 8;
    src.texY = proc->unk34 << 8;
    src.scrX = proc->unk30 - 0x40;
    src.scrY = proc->unk34 + proc->unk2c;
    src.sx = b;
    src.sy = b;
    src.alpha = a;

    BgAffineSet(&src, (struct BgAffineDstData *)gUnknown_030024D0, 1);

    if (proc->unk54 == proc->unk48)
        Proc_Break(proc);

    proc->unk54++;
}
asm(".global sub_08067948\n.thumb_set sub_08067948, IntroBgAffineTween_Loop\n");

void StartIntroBgAffineTween(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
                  int a9, ProcPtr parent)
{
    struct Unk08581014Proc *p;

    p = Proc_Start(gUnknown_08581014, parent);

    p->unk2c = a2 << 4;
    p->unk30 = a3;
    p->unk34 = a4;
    p->unk38 = a5;
    p->unk3c = a6;
    p->unk40 = a7;
    p->unk44 = a8;
    p->unk48 = a9;
    p->unk4c = a1;
    p->unk54 = 0;
    p->unk50 = 0;
}
asm(".global sub_080679D8\n.thumb_set sub_080679D8, StartIntroBgAffineTween\n");
