#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806717C.
 * sub_0806717C @ 0x0806717C
 */

#include "hardware.h"
#include "proc.h"
struct Unk6717CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ u32 unk58;
};

/* The five target1 layer bits are written as one HImode read-modify-write on
 * the raw halfword, not as five bitfield stores: a 5-bit field at bit 0 lives
 * inside byte 0, so get_best_mode would have picked QImode and emitted
 * `ldrb`/`strb`, and an all-ones bitfield value drops the AND altogether. The
 * `ldrh`/`ldr =0xffe0`/`ands`/`orrs`/`strh` sequence can only come from the
 * raw member. target1_enable_bd, by contrast, really is a single-bit field.
 */
void BlendFromWhite_Init(struct Unk6717CProc *proc)
{
    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0x1f;

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 0x1f;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk58 = 0x1f;
}
asm(".global sub_0806717C\n.thumb_set sub_0806717C, BlendFromWhite_Init\n");
