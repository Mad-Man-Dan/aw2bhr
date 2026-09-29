#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011588.
 * sub_08011588 @ 0x08011588, sub_080115B4 @ 0x080115B4
 */

#include "hardware.h"
#include "proc.h"
struct Unk11588Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk115B4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

/* StartFadeToBlack plus the blend write. `(x & 0x3f) | 0x80` really is the
 * bitfield store `effect = 2`: the complement mask ~0xC0 is 0x3F in QImode,
 * whose sign bit is clear, so it materialises as a bare `movs #0x3f` rather
 * than the `mov #N; neg` pair a mask with bit 7 set would need. */
void StartFadeToWhite(int a)
{
    struct Unk11588Proc *proc = Proc_Start(gUnknown_0848923C, PROC_TREE_3);

    proc->unk64 = a;
    gUnknown_030030E0.bits.effect = 2;
}
asm(".global sub_08011588\n.thumb_set sub_08011588, StartFadeToWhite\n");

void StartFadeFromWhite(int a)
{
    struct Unk115B4Proc *proc = Proc_Start(ProcScr_DesignRoomFadeIn, PROC_TREE_3);

    proc->unk64 = a;
    gUnknown_030030E0.bits.effect = 2;
}
asm(".global sub_080115B4\n.thumb_set sub_080115B4, StartFadeFromWhite\n");
