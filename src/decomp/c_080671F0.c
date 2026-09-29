#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080671F0.
 * sub_080671F0 @ 0x080671F0, sub_0806724C @ 0x0806724C
 */

#include "hardware.h"
#include "proc.h"
struct Unk671F0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ u32 unk30;
    /* 34 */ u32 unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x3c);
    /* 3c */ u32 unk3c;
};
struct Unk6724CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ u32 unk30;
    /* 34 */ u32 unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x3c);
    /* 3c */ u32 unk3c;
};

/* src/decomp/c_08067300.c with `effect = 2` instead of 3 and different
 * unk30/unk34 constants -- everything else, including the `do { } while (0)`
 * code-motion barrier around the blend-target group, is the same source.
 *
 * `effect = 2` masks with 0x3f first where BlendRampBlack0To16_Init's `= 3` does not: 3
 * fills the two-bit field, so store_fixed_bit_field sets all_one and drops the
 * AND. That asymmetry between the two functions is the compiler's, not the
 * source's.
 *
 * The group write must stay three statements -- mask, build into a local, OR
 * in -- because fold would otherwise reassociate `(raw & 0xffe0) |
 * ((unk3c << 4) | 0xf)` and put the `movs #0xf` before the shift. */
void BlendRampWhite8To0_Init(struct Unk671F0Proc *proc)
{
    u32 v;

    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 8;

    do
    {
        gUnknown_030030E0.raw &= 0xffe0;
        v = (proc->unk3c << 4) | 0xf;
        gUnknown_030030E0.raw |= v;
    } while (0);

    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk30 = 8;
    proc->unk34 = 0;
}
asm(".global sub_080671F0\n.thumb_set sub_080671F0, BlendRampWhite8To0_Init\n");

/* Character-for-character src/decomp/c_08067300.c except for `effect = 2`
 * in place of `effect = 3`. The two functions are 92 and 88 bytes; the four
 * bytes are the `movs #0x3f; ands` that a non-full bitfield value needs. */
void BlendRampWhite0To16_Init(struct Unk6724CProc *proc)
{
    u32 v;

    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    do
    {
        gUnknown_030030E0.raw &= 0xffe0;
        v = (proc->unk3c << 4) | 0xf;
        gUnknown_030030E0.raw |= v;
    } while (0);

    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk30 = 0;
    proc->unk34 = 0x10;
}
asm(".global sub_0806724C\n.thumb_set sub_0806724C, BlendRampWhite0To16_Init\n");
