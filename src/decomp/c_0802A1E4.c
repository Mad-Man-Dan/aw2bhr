#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A1E4.
 * sub_0802A1E4 @ 0x0802A1E4, sub_0802A258 @ 0x0802A258
 */

/* The map planes go through a LOCAL struct cast onto gUnknown_08499590 (the
 * W34-F spelling recorded in include/unknown-globals.h): only a COMPONENT_REF
 * keeps the ROM's `(map + K) + idx` association instead of folding K into the
 * load displacement.
 *
 * The row lookup has to be the LEFT operand of the `+`. Written `x + rows[y]`
 * the code is the same size and every instruction is the same, but agbcc
 * materialises x's sign extension as a split `lsls` early / `asrs` late pair
 * instead of the adjacent pair the ROM has. Same operand-order rule as
 * c_0802966C.c.
 *
 * The return is s16, NOT bool8: the only caller (HasAdjacentOwnSupplyUnit) truth-tests it
 * with `lsls #0x10`, and the body's `movs r0, #1` / `movs r0, #0` is identical
 * at every width. */
/* WAVE 35: CANONICAL `struct Map`. Eight drafts across blocks 0x08029-0x0802B
 * each invented their own body for this tag, with 2 to 7 named fields. Every
 * one compiles and byte-matches ALONE, so trymatch cannot see the problem;
 * merged into a promoted unit it is a hard `redefinition of struct Map`.
 * All seven distinct bodies describe the SAME layout at different
 * resolutions -- the offsets reconcile exactly -- so this is their union, the
 * most refined reading of each region. Byte-neutral: only a field's START
 * OFFSET enters the address arithmetic, never its declared length, and no
 * draft referenced any filler. Keep the drafts in sync; sync_work.py
 * reintroduces whatever the drafts say. */
struct Unk2A258
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x01];
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
};

s16 IsOwnSupplyUnitAtCell(s16 x, s16 y)
{
    struct Unit *unit;

    unit = &gUnits[gMap->unit[
        gMap->rowOffset[y] + x]];

    if (gUnknown_03003F2C == ((unit - gUnits) & 0xc0)
        && HasSupplyAbility((u8 *)unit))
        return 1;

    return 0;
}
asm(".global sub_0802A1E4\n.thumb_set sub_0802A1E4, IsOwnSupplyUnitAtCell\n");

bool8 HasAdjacentOwnSupplyUnit(struct Unk2A258 *p)
{
    if (gUnknown_084995A8[p->unk00] == 0)
        return 0;

    if (p->unk02 != 0 && IsOwnSupplyUnitAtCell(p->unk02 - 1, p->unk03))
        return 1;

    if (p->unk03 != 0 && IsOwnSupplyUnitAtCell(p->unk02, p->unk03 - 1))
        return 1;

    if (p->unk02 < gMap->width - 1
        && IsOwnSupplyUnitAtCell(p->unk02 + 1, p->unk03))
        return 1;

    if (p->unk03 < gMap->height - 1
        && IsOwnSupplyUnitAtCell(p->unk02, p->unk03 + 1))
        return 1;

    return 0;
}
asm(".global sub_0802A258\n.thumb_set sub_0802A258, HasAdjacentOwnSupplyUnit\n");
