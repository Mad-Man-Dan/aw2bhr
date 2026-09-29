#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B690.
 * sub_0807B690 @ 0x0807B690
 */

#include "hardware.h"
#include "proc.h"
/* MATCHED, wave 48 (W48-E).
 *
 * PROMOTION NEEDS A .rodata ENTRY -- this does not promote correctly without
 * it. try_match reports `relocs: name different symbols that resolve to the
 * same address` at +0x09c, original gUnknown_081D9328 against candidate
 * .rodata. Add to this function's data/promoted.json entry:
 *
 *     "rodata": ["0x081D9328"]
 *
 * then re-run tools/split_rodata.py and tools/gen_lds.py before building.
 *
 * gUnknown_081D9328 is NOT a global. It is this function's own -fforce-addr
 * address-constant pool word, and it dereferences in baserom.gba to
 * 0x0848B6C6, the OAM blob the two later PutSprite calls pass. 0x081D9324
 * holds the SAME address as a neighbouring function's private copy, which is
 * the giveaway. Naming gUnknown_0848B6C6 directly is the honest spelling and
 * it reproduces the ROM's `ldr r4, =pool; ldr r3, [r4]` shape, including the
 * reload inside the loop.
 *
 * `(u16)proc->unk_38` is a cast, not a narrow member: the ROM reads +0x38 with
 * `ldrh` here and with a full `ldr`/`adds #1`/`str` two statements later, so
 * the field is `int` and this access narrows it.
 *
 * The `off = ... * 2` local is load-bearing -- see the "`ldr` BEFORE the index
 * shift" chapter of docs/agbcc-codegen.md. Every subscript spelling puts the
 * pool `ldr` ahead of the `lsls` and misses by 4 bytes.
 *
 * sub_0807B738 was RETYPED from `struct Proc7B738 *` to ProcPtr in
 * src/decomp/c_0807B738.c so it could be declared in unknown-functions.h for
 * this call. It was re-verified with try_match and still matches.
 */
struct Proc7B690
{
    u8 filler_00[0x38];
    /* 0x38 */ int unk_38;
    u8 filler_3c[0x1c];
    /* 0x58 */ int unk_58;
    /* 0x5C */ int unk_5c;
};

void MatchSummaryPanel_Hold_Loop(struct Proc7B690 *proc)
{
    int off;
    int i;

    off = DivRem(Div((u16)proc->unk_38, 3), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_0822AC60 + off), 0x238, 2);

    proc->unk_38++;

    PutSpriteNumberRightAligned(0x5b, 0x54, proc->unk_58, 0);
    PutSpriteNumberRightAligned(0x5b, 0x67, proc->unk_5c, 1);

    PutSprite(0, 0x14, 0x54, gUnknown_0848B690, 0x1058);
    PutSprite(0, 0xa, 0x67, gUnknown_0848B6C6, 0x105c);

    for (i = 0; i < 2; i++)
        PutSprite(0, 0x6c, 0x54 + i * 0x13, gUnknown_0848B6C6, 0x1050);

    sub_0807B738(proc);
}
asm(".global sub_0807B690\n.thumb_set sub_0807B690, MatchSummaryPanel_Hold_Loop\n");
