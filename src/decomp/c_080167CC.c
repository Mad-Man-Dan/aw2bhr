#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080167CC.
 * sub_080167CC @ 0x080167CC, sub_080167F8 @ 0x080167F8
 */

/* One command of the gUnknown_03001470 script stream, the same eight-byte
 * advance as src/decomp/c_08015F10.c's pair: the operand word is dereferenced
 * once and handed to sub_080156E8. `gUnknown_03001470[a].unk04` is NAMED TWICE
 * rather than bound to a pointer local -- see the note in c_08015E80.c -- which
 * is what produces the clean `=gUnknown_03001470` pool word plus `adds r1,#4`
 * ahead of the index add. */
bool8 sub_080167CC(u8 a)
{
    sub_080156E8(a, *(void **)gUnknown_03001470[a].unk04);
    gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
    return TRUE;
}

/* sub_080167CC's neighbour, one command along: the operand is the UNSIGNED
 * halfword at +4 of the 8-byte command (`ldrh r1,[r1,#4]`), which agrees with
 * SetSlotSpriteScriptIndex's declared `u16` second parameter. Its signed twin is
 * src/decomp/c_08015EAC.c's SlotOp_EndSlotAt, which uses `ldrsh` for an s16
 * callee. */
bool8 sub_080167F8(u8 a)
{
    SetSlotSpriteScriptIndex(a, ((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u8 *)gUnknown_03001470[a].unk04 + 8;
    return TRUE;
}
