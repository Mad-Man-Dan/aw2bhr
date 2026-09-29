#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08068770.
 * sub_08068770 @ 0x08068770
 */

#include "proc.h"
/* One frame of the 0x08581210 menu's sprite draw: put `proc->unk2a` sprites in
 * a row 0x22 apart, from an origin that wobbles on a 5-phase cycle, and break
 * the proc once the frame counter at +0x4f reaches the length at +0x4e.
 *
 * `proc->unk4f % 10 / 2` is UNSIGNED (`__umodsi3`, not `__modsi3`) with no cast
 * in sight: unk4f is a u8, both operands fit in `unsigned char`, and
 * shorten_binary_op narrows TRUNC_MOD_EXPR the same way it narrows MULT_EXPR.
 * The `/ 2` on that narrowed result is the `lsls #24; lsrs #25` pair -- read it
 * as `(u8)x >> 1`, i.e. combine folding the u8 truncation into the shift, not
 * as a mask.
 *
 * gUnknown_08581248 is indexed FLAT (`[k * 2]`, `[k * 2 + 1]`) rather than
 * declared `s16 [][2]`; see the note on the declaration in
 * include/unknown-globals.h for the two instructions that separate them.
 *
 * `y = proc->unk39 + tbl[...]` has its operands in that order even though the
 * ROM's `adds r7, r0, r3` adds the table value to the field: `fold`
 * canonicalises the sum with the more complex operand first, so the ADD reads
 * table-then-field either way, but `-fforce-addr` materialises each operand's
 * address in SOURCE order, and only this order puts `adds r3, r6, #0; adds r3,
 * #0x39` ahead of the index shifts. Written `tbl[...] + proc->unk39` the two
 * are the same instructions in the other order. So on a sum of two memory
 * operands the ADD's register order tells you nothing about the source and the
 * ADDRESS setup order tells you everything. */

struct Unk68770Proc
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x05];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 filler_33[0x05];
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3a */ u8 filler_3a[0x14];
    /* 0x4e */ u8 unk4e;
    /* 0x4f */ u8 unk4f;
};

void IntroCoNameWobble_Loop(struct Unk68770Proc *proc)
{
    int i;
    int x;
    int y;

    x = proc->unk38 + gUnknown_08581248[proc->unk4f % 10 / 2 * 2];
    y = proc->unk39 + gUnknown_08581248[proc->unk4f % 10 / 2 * 2 + 1];

    for (i = 0; i < proc->unk2a; i++)
    {
        PutSprite(0, x + i * 0x22, y, gUnknown_0858125C,
                  i * 16 + proc->unk30 + (proc->unk32 << 12));
    }

    proc->unk4f++;

    if (proc->unk4f == proc->unk4e)
    {
        Proc_Break(proc);
    }
}
asm(".global sub_08068770\n.thumb_set sub_08068770, IntroCoNameWobble_Loop\n");
