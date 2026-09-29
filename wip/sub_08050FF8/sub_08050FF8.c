#include "global.h"

/* Sets up the current sprite object for one side and slot, then places it.
 *
 * The sprite's tile number comes from the OTHER side's record. The function
 * marks the slot as used, fills in the object's record fields, works out the
 * sprite's palette and priority, and hands the object to sub_08015608. It then
 * computes the sprite's x and y from the side's column table and a row of
 * per-frame offsets, steps that frame counter (wrapping at 3), and places the
 * sprite with sub_08050528. When the other side's unit has a certain flag set
 * it also plays an alternating effect through sub_0803B48C.
 *
 * Why the C looks odd, and all of it is needed to match:
 *
 *  - The four globals at 0x03004580, 0x0300453C, 0x020298E0 and 0x085D6A48 are
 *    reached through the read-only cells that hold their addresses, with each
 *    cell's ADDRESS bound to a local and the value re-loaded at every use.
 *    Naming those globals directly makes the compiler fold the base into each
 *    load and the code comes out much shorter than the original.
 *  - The column addresses are built in two statements (bind the row, then add
 *    the column) because a single expression lets the compiler absorb the
 *    column offset into the load instead.
 *  - The row index is bound already scaled to BYTES, so the reference has no
 *    scaling left to apply.
 *  - The side variable is read through a volatile pointer, because the original
 *    re-reads it from memory at every use rather than keeping it in a register.
 */

struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x02];
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u8 filler_0a[0x0a];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 filler_16[0x02];
};

struct Unk02029664Bits
{
    u32 bit0 : 1;
    u32 filler_01 : 2;
    u32 bit3 : 1;
    u32 filler_04 : 28;
};

#define SIDE (*(volatile u16 *)&gUnknown_0300453C)

void sub_08050FF8(void)
{
    struct OamData oam;
    u16 x;
    u16 y;
    u16 priority;
    u16 (*const *pE4)[8];
    u16 *const *pDC;
    struct Unk020298E0 *const *pD8;
    u16 *p451C;
    u16 (*e4)[8];
    u16 (*e4b)[8];
    u16 *c1;
    u16 *c1b;
    u16 *c2;
    s16 t;
    u16 lv0;
    int k;
    int off;
    int offb;

    sub_0801566C(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_02029906[SIDE]
                     [gUnknown_020298E0[SIDE].unk16 - 1] = 1;
    gUnknown_03001470[gUnknown_03001FBC].unk28 =
        gUnknown_020298E0[SIDE].unk16 - 1;
    gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = SIDE;
    p451C = &gUnknown_0300451C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = *p451C;

    priority = gUnknown_08552394[
        ((struct Unk02029664Bits *)&gUnknown_02029664)->bit0
        + ((struct Unk02029664Bits *)&gUnknown_02029664)->bit3];
    oam.priority = priority;
    oam.hFlip = SIDE;
    oam.paletteNum = 8;
    oam.tileNum = gUnknown_020298E0[SIDE ^ 1].unk00;

    sub_08015608(gUnknown_03001FBC, *(struct UnkVec *)&oam);

    pE4 = &gUnknown_081360E4;

    e4 = *pE4;
    off = SIDE * 16;
    c1 = (u16 *)e4 + 1;

    t = ((struct Unk85D6A48Row *)gUnknown_081360E0)
            [*(u16 *)((u8 *)c1 + off)].unk04;

    pDC = &gUnknown_081360DC;
    pD8 = &gUnknown_081360D8;

    if (t == 0)
        gUnknown_02029906[SIDE][*p451C] = 1;
    else
        gUnknown_02029906[SIDE]
                         [gUnknown_08552148[SIDE]] = 1;

    e4 = *pE4;
    c2 = (u16 *)e4 + 2;
    k = (**pDC ^ 1) * 8;

    if (c2[k] == 1)
    {
        c1 = (u16 *)e4 + 1;
        if (c1[k] == 0xD)
            sub_0803B48C(gUnknown_085643A8[1]
                [(*pD8)[**pDC].unk8c & 1]);
        else
            sub_0803B48C(gUnknown_085643A8
                [c2[(**pDC ^ 1) * 8] - 1]
                [(*pD8)[**pDC].unk8c & 1]);
        (*pD8)[**pDC].unk8c++;
    }

    e4b = *pE4;
    offb = **pDC * 16;
    c1b = (u16 *)e4b + 1;

    lv0 = ((struct Unk85D6A48Row *)gUnknown_085D6A48)
              [*(u16 *)((u8 *)c1b + offb)].unk08;
    x = gUnknown_02029A10[**pDC].entries[*p451C].x
        + gUnknown_08553B58[**pDC]
        + gUnknown_08553B5C[lv0][**pDC];
    y = gUnknown_02029A10[**pDC].entries[*p451C].y
        + gUnknown_08553BFC[gUnknown_020298E0[**pDC].unk18].unk04
        - 8;
    gUnknown_020298E0[**pDC].unk18++;
    if (gUnknown_020298E0[**pDC].unk18 == 3)
        gUnknown_020298E0[**pDC].unk18 = 0;

    sub_08050528(**pDC, gUnknown_03001FBC, x, y);
}
