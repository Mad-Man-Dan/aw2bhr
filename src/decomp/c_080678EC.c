#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080678EC.
 * sub_080678EC @ 0x080678EC
 */

#include "hardware.h"
#include "proc.h"
struct Unk678ECProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x40);
    /* 40 */ int unk40;
};

/* Builds one BIOS affine descriptor per frame from the proc's camera fields.
 * The 0x14-byte stack frame is exactly `struct BgAffineSrcData` from
 * include/gba/types.h -- two words, four halfwords, one trailing halfword, in
 * that order -- which is what identifies the callee as BgAffineSet and settles
 * the argument types; the ROM's stores land on texX/texY, scrX/scrY/sx/sy and
 * alpha with nothing left over.
 *
 * gUnknown_030024D0 is hardware.h's `volatile u32 [4]`, i.e. one
 * BgAffineDstData's worth of BG2 affine registers. It is cast at the call
 * rather than retyped, because its other readers (FlushDisplayRegisters copies it a
 * word at a time) depend on the array spelling. */
void IntroBgAffineTween_Init(struct Unk678ECProc *proc)
{
    struct BgAffineSrcData src;

    src.texX = proc->unk30 << 8;
    src.texY = proc->unk34 << 8;
    src.scrX = proc->unk30 - 0x40;
    src.scrY = proc->unk34 + proc->unk2c;
    src.sx = proc->unk40;
    src.sy = proc->unk40;
    src.alpha = proc->unk38;

    BgAffineSet(&src, (struct BgAffineDstData *)gUnknown_030024D0, 1);
}
asm(".global sub_080678EC\n.thumb_set sub_080678EC, IntroBgAffineTween_Init\n");
