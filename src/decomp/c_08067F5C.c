#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067F5C.
 * sub_08067F5C @ 0x08067F5C
 */

#include "hardware.h"
#include "proc.h"
struct Unk67F5CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
};

/* Two AND masks survive side by side (0xffe0 then 0xe0ff) instead of folding
 * to one 0xe0e0: on THUMB each is a pool `ldr`, so combine never gets a bare
 * constant to substitute, and tree-level folding cannot merge them because
 * they belong to two different statements. The first store is dead and is
 * eliminated, which is why only one `strh` comes out.
 */
void IntroBlendFade_Init(struct Unk67F5CProc *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = proc->unk2c;
    gUnknown_03002B28 = 0x10 - proc->unk2c;
    gUnknown_03001FFC = 0;

    gUnknown_030030E0.raw &= 0xffe0;
    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xe0ff) | 0xf00;
    gUnknown_030030E0.bits.target2_enable_bd = 1;
}
asm(".global sub_08067F5C\n.thumb_set sub_08067F5C, IntroBlendFade_Init\n");
