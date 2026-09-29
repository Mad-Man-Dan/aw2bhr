#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067300.
 * sub_08067300 @ 0x08067300
 */

#include "hardware.h"
#include "proc.h"
struct Unk67300Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ u32 unk30;
    /* 34 */ u32 unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x3c);
    /* 3c */ u32 unk3c;
};

/* The blend-target write is three statements, not one expression: fold would
 * reassociate `(raw & 0xffe0) | ((unk3c << 4) | 0xf)` into `(raw & 0xffe0 |
 * 0xf) | (unk3c << 4)`, which puts the `movs #0xf` before the shift instead of
 * after it. Masking first, then building the value into a local, then ORing it
 * in, is the only spelling that keeps the ROM's order. The `do { } while (0)`
 * around the group is the same code-motion barrier BlendToWhite_Init needs: without
 * it the final `proc->unk30 = 0` is materialised early, inside the group, and
 * takes a second callee-saved register.
 *
 * `effect = 3` fills the two-bit field, so store_fixed_bit_field sets all_one
 * and drops the AND -- that is why this one is `orr #0xc0` alone where its
 * neighbours mask with 0x3f first.
 */
void BlendRampBlack0To16_Init(struct Unk67300Proc *proc)
{
    u32 v;

    gUnknown_030030E0.bits.effect = 3;

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
asm(".global sub_08067300\n.thumb_set sub_08067300, BlendRampBlack0To16_Init\n");
