#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C7B4.
 * sub_0806C7B4 @ 0x0806C7B4
 */

#include "hardware.h"
/* The blend-shadow family described on gUnknown_030030E0 in hardware.h. The
 * two 5-bit target groups go through the SCALAR `u16 *` cast, not through
 * `.raw`: the union spelling drags the live zero from `gUnknown_03002B28 = 0`
 * into a spurious `orr` and costs a register -- the same failure c_08085F40.c
 * records. The single backdrop bits either side of them are genuine QImode
 * bitfields (`ldrb`, `movs #0x21; neg` mask).
 *
 * `gUnknown_03001FFC = gUnknown_03001FFC;` is a real self-store and is only
 * spellable because that global is volatile; without the qualifier the whole
 * statement is deleted and the `ldrh`/`strh` pair disappears. */

struct Unk806C7B4
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void CreditsResultFade_Init(struct Unk806C7B4 *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = gUnknown_03001FFC;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x10;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0xF00;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    proc->unk58 = 0;
}
asm(".global sub_0806C7B4\n.thumb_set sub_0806C7B4, CreditsResultFade_Init\n");
