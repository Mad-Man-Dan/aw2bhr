#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067E88.
 * sub_08067E88 @ 0x08067E88
 */

#include "proc.h"
struct Unk67E88Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ u8 unk29;
    /* 2a */ u8 unk2a;
    /* 2b */ STRUCT_PAD(0x2b, 0x30);
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x38);
    /* 38 */ int unk38;
    /* 3c */ u8 unk3c;
};

/* Puts one OBJ out for the entry IntroCoSlide_Init loaded, sharing its +0x2a index
 * into gUnknown_08581104. The third argument is the OAM attribute-2 word built
 * the way the ROM builds it: 0x2000 (`movs #0x80; lsls #6`) OR the tile id OR
 * the palette at bit 10, left-associative, which is exactly the two `orrs`.
 * 0x4a0 is `movs #0x94; lsls #3`, and 0x1ff comes from the pool.
 *
 * The fifth argument goes on the stack (`str r0,[sp]`); it is a u8 field
 * reaching sub_08043C28's u8 parameter, so the `ldrb` is the whole conversion
 * and no PROMOTE_MODE pair appears. */
void IntroCoSlide_HoldLoop(struct Unk67E88Proc *proc)
{
    sub_08043C28(proc->unk30 & 0x1ff, 0x4a0,
                 0x2000 | gUnknown_08581104[proc->unk2a] | (proc->unk38 << 10),
                 0, proc->unk3c);
}
asm(".global sub_08067E88\n.thumb_set sub_08067E88, IntroCoSlide_HoldLoop\n");
