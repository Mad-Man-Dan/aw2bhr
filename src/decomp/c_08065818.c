#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065818.
 * sub_08065818 @ 0x08065818
 */

/* gUnknown_0816E10C is agbcc's own -fforce-addr word holding &gUnknown_08580934
 * (the ROM word at that address is 0x08580934), so naming the global honestly
 * gives the ROM's three-level `ldr rN,=<word>; ldr r5,[rN]; ldr r1,[r5]`.
 *
 * The +0x88 field is spelled `unk07 + (1 - unk15)`, and the parentheses are the
 * whole difference. Written `unk07 + 1 - unk15` fold associates it into
 * `unk07 - (unk15 - 1)` and emits `ldrb; subs #1; subs` -- same size, wrong
 * three instructions. `unk07 - unk15 + 1` gives `subs; adds`. Only the form
 * that keeps the constant on the LEFT operand's side survives fold as the
 * ROM's `adds #1; ldrb; subs`. */
void MatchSetupPackRuleIndices(void)
{
    gUnknown_08580934->unk16--;
    gUnknown_08580934->unk84 = gUnknown_08580934->unk00 == 0;
    gUnknown_08580934->unk85 = gUnknown_08580934->unk06;
    gUnknown_08580934->unk86 = gUnknown_08580934->unk04 / 500 - 2;
    gUnknown_08580934->unk87 = gUnknown_08580934->unk03 ? gUnknown_08580934->unk03 - 4 : 0;
    gUnknown_08580934->unk88 = gUnknown_08580934->unk07 ? gUnknown_08580934->unk07 + (1 - gUnknown_08580934->unk15) : 0;
    gUnknown_08580934->unk89 = gUnknown_08580934->unk01 == 0;
    gUnknown_08580934->unk8a = gUnknown_08580934->unk02;
}
asm(".global sub_08065818\n.thumb_set sub_08065818, MatchSetupPackRuleIndices\n");
