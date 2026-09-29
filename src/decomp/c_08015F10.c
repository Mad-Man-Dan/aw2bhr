#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015F10.
 * sub_08015F10 @ 0x08015F10, sub_08015F3C @ 0x08015F3C
 */

/* Family F067, second member -- see SlotOp_StartEventScript for why `gUnknown_03001470[i]
 * .unk04` has to be named twice rather than bound to a pointer local.
 *
 * The operand's C type is the only thing that changes with the callee, and it
 * changes for a real reason: sub_080153B8 compares its argument against the
 * slot's own ADDRESS (`adds r0,r0,r2; cmp r0,r1` over `&gUnknown_03001470[i]`),
 * so this command's operand word is a slot pointer, where SlotOp_StartEventScript's is a
 * script pointer and SlotOp_EndScript's is the blob sub_0801537C scans for. All
 * three are `ldr r0,[r0]` and the cast is byte-neutral; it is written honestly
 * because the callee declarations are what record the difference. */

bool8 SlotOp_EndSlotByPointer(u8 i)
{
    sub_080153B8(*(struct Unk03001470 **)gUnknown_03001470[i].unk04);
    gUnknown_03001470[i].unk04 = (const u8 *)gUnknown_03001470[i].unk04 + 8;
    return 1;
}
asm(".global sub_08015F10\n.thumb_set sub_08015F10, SlotOp_EndSlotByPointer\n");

/* Family F067, third member -- see SlotOp_StartEventScript for the double-naming rule
 * that produces the clean `=gUnknown_03001470` pool word plus `adds r1,#4`.
 * The operand here is a script blob, the same thing every other sub_0801537C
 * call site in the tree passes (`const void *`); the result is discarded, so
 * this call site says nothing about sub_0801537C's undecided return width. */

bool8 SlotOp_EndScript(u8 i)
{
    sub_0801537C(*(const void **)gUnknown_03001470[i].unk04);
    gUnknown_03001470[i].unk04 = (const u8 *)gUnknown_03001470[i].unk04 + 8;
    return 1;
}
asm(".global sub_08015F3C\n.thumb_set sub_08015F3C, SlotOp_EndScript\n");
