#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050424.
 * sub_08050424 @ 0x08050424
 */

/* The whole function is three statements plus the call. Two shift pairs are net
 * LEFT shifts with a u16 truncation and must be written as MULTIPLIES:
 * `a3 * 0x200` from `lsls #0x19; lsrs #0x10` and `* 0x20` from `lsls #0x15;
 * lsrs #0x10`. The third parameter is `int`, not u16 -- src/decomp/c_0805040C.c
 * forwards a u16 `c` and its own third lsls/lsrs pair belongs to that wrapper.
 *
 * The one lever is the COMMA ANCHOR. The ROM materialises gUnknown_085D6A48's
 * address BETWEEN `a1 * 0x100` and the scaled row index, which no plain address
 * expression reaches: fold canonicalises PLUS(ADDR_EXPR, MULT_EXPR) so the
 * index becomes operand 0 and the pool ldr lands after it. Binding the base to
 * a local in its own statement puts the ldr at the TOP of the function instead
 * (measured: registers then match the ROM exactly, only the ldr is five
 * instructions early). The comma inside operand 1 of the sum creates the
 * reference at a point no statement boundary can reach, and that is exact.
 *
 * The struct is what keeps the column offset in the load displacement --
 * `gUnknown_085D6A48[j][9]` on the declared `u16 [][12]` emits
 * `adds rB,#0x12; ldrh [rB]` where the ROM has `ldrh [rB, #0x12]`. */
struct Unk85D6A48Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 filler_02[0x10];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[4];
};

void StreamWholePose(u16 a1, u16 a2, int a3)
{
    struct Unk85D6A48Row *tbl;
    u16 j;
    u16 src;
    u16 off;
    u16 idx;

    j = gUnknown_03004580[a1][1];
    src = a3 * 0x200;
    off = (a1 * 0x100
           + a2 * (tbl = (struct Unk85D6A48Row *)gUnknown_085D6A48, tbl)[j].unk12)
          * 0x20;
    idx = gUnknown_08551D1C[gUnknown_02029A10[a1].entries[a2].unk00];

    RegisterDataMove((u8 *)gUnknown_02029BA8[a1].unk18[idx] + src,
                 (void *)(0x06010000 + off), 0x200);
}
asm(".global sub_08050424\n.thumb_set sub_08050424, StreamWholePose\n");
