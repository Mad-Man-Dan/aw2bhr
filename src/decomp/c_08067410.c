#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067410.
 * sub_08067410 @ 0x08067410
 */

#include "hardware.h"
#include "proc.h"
struct Unk67410Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ u32 unk58;
};

/* Same body as BlendFromWhite_Init with the blend_y and the proc field zeroed instead
 * of 0x1f. The `do { } while (0)` around the raw read-modify-write is not
 * decoration: without it agbcc materialises the final `proc->unk58 = 0` early,
 * inside that statement, which costs a callee-saved register and four bytes of
 * push/pop. The degenerate loop is a code-motion barrier and pins the `movs
 * r1, #0` to its own store. See MatchSetupHighlightSelectedRuleOption for the other function in this
 * batch that needs the same construct, for the opposite effect.
 */
void BlendToWhite_Init(struct Unk67410Proc *proc)
{
    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;

    do { gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 0x1f; } while (0);
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    proc->unk58 = 0;
}
asm(".global sub_08067410\n.thumb_set sub_08067410, BlendToWhite_Init\n");
