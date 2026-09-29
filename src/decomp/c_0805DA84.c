#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DA84.
 * sub_0805DA84 @ 0x0805DA84
 */

/* Picks a unit with AiPickSupplyWard, finds a destination cell next to it with
 * AiPickFiringCellBesideUnit, clears that unit's three-bit +0x09 field and hands the cell to
 * AiPublishAction. Does nothing if either step comes back empty.
 *
 * The scratch cell is a four-byte struct local, so the 0x270F seed is a
 * bitfield insert (`ldr; and #0xFFFF0000; orr; str`) and the sentinel must be
 * spelled as a literal at both sites rather than bound -- see sub_0805A854 for
 * the measurement; a binding statement moves the constant's `ldr` ahead of the
 * `and` and swaps the two pool words.
 *
 * `gUnits + id` AND NOT `&gUnits[id]`, and this is the
 * whole remaining difference between the two spellings. Both emit seven
 * instructions and the same *12 shift-add chain, but the ADDR_EXPR form
 * expands the array base first, so the pointer load lands immediately after
 * its own pool word (`ldr rA,=g; ldr rA,[rA]; <index>; add`). The pointer-sum
 * form leaves the pool word's register live across the index computation and
 * sinks the dereference to just before the add, which is the ROM's
 * `ldr r2,=g; ldr r1,[sp,#4]; <index>; ldr r1,[r2]; adds r1,r1,r0`. Binding
 * the sum to a local is what holds the association; the cast alone does not.
 *
 * The +0x09 field is written through a locally declared bitfield view, the
 * same idiom src/decomp/c_08061E54.c and c_08061E80.c use for this exact
 * field. It is what produces the ROM's SImode `movs r0,#8; rsbs r0,r0,#0;
 * ands` -- `unk09 &= ~7` on the plain `u8` member shortens the mask to 0xf8
 * and does not match. struct Unit's unk09 is left alone so the layout
 * stays shared.
 *
 * AiPublishAction's first two arguments come out as `movs rK,#0; ldrsh` off the
 * `u16` pair because the parameters are `s16`; the third is the literal 6, the
 * fourth is `(u8)id` from the declared `u8` parameter, and the fifth is the
 * stack word. */
struct CellXY
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};
struct Unk5DA84
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
    /* 0x09 */ u8 unk09_3 : 5;
};

void AiDeliberateSupplyInReach(void)
{
    int id;
    struct CellXY pos;
    struct Unk5DA84 * u;

    GenerateUnitMovementMap(gUnknown_030040D8);
    MapMarkHalo(0x79);
    AiPickSupplyWard(&id);

    if (id == -1)
        return;

    pos.x = 0x270F;

    AiPickFiringCellBesideUnit(id, (u16 *)&pos);

    if (pos.x == 0x270F)
        return;

    u = (struct Unk5DA84 *)(gUnits + id);
    u->unk09_0 = 0;

    AiPublishAction(pos.x, pos.y, 6, id, 0);
}
asm(".global sub_0805DA84\n.thumb_set sub_0805DA84, AiDeliberateSupplyInReach\n");
