#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080228D8.
 * sub_080228D8 @ 0x080228D8
 */

/* WAVE 79 (W79-C) -- MATCHED. Two constructs, both recorded in
 * docs/agbcc-codegen.md:
 *
 * 1. The index local is scoped PER ARM. A single function-scope local hoists
 *    the whole `__umodsi3` above the branch and costs 16 bytes (that is what
 *    the wave-32 draft did, and its "size-exact" measurement was best.c's, not
 *    the draft's). A block-scoped local in each arm keeps both computations.
 *
 * 2. `(u16 *)(i * 2 + (u32)g)` rather than `g + i`. C pointer arithmetic goes
 *    through pointer_int_sum, which always builds PLUS_EXPR(ptr, intop) in
 *    that order, so expand emits the base's pool `ldr` BEFORE the index's
 *    `lsls`. Doing the arithmetic in u32 makes it an ordinary commutative
 *    PLUS_EXPR, and because `&g` is TREE_CONSTANT fold moves it to operand 1 --
 *    so the `lsls` is expanded first and the pool `ldr` lands after it, which
 *    is the ROM's order. `g + i`, `&g[i]`, `i + g`, `(u8 *)g + i * 2` and a
 *    bound `u16 *p` all keep the pointer-sum order and are 8 bytes wrong. */
struct Unk80228D8Proc
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ s16 unk20;
};

void RangeOverlayScript_Loop(struct Unk80228D8Proc *proc)
{
    if (proc->unk20 <= 7)
    {
        RegisterDataMove(gUnknown_0809181C + proc->unk20 * 0x80, (void *)0x06003600, 0x80);
        proc->unk20++;
    }

    if (gUnknown_03001470[gUnknown_03001FBC].unk1e == 0)
    {
        u32 i;

        i = ((u32)gGameClock >> 2) % 15;
        sub_08013664((u16 *)(i * 2 + (u32)gUnknown_08091C9E), 0x102, 0x1e);
    }
    else
    {
        u32 i;

        i = ((u32)gGameClock >> 2) % 15;
        sub_08013664((u16 *)(i * 2 + (u32)gUnknown_08091C5E), 0x102, 0x1e);
    }
}
asm(".global sub_080228D8\n.thumb_set sub_080228D8, RangeOverlayScript_Loop\n");
