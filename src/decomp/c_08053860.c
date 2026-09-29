#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053860.
 * sub_08053860 @ 0x08053860, sub_08053BB8 @ 0x08053BB8
 */

/* One of a mirrored pair with MovePanelsBack: forty proc ids out of four
 * parallel tables, each handed to SetSlotSpritePriority with the palette that goes with
 * its half of the table. The tables are two-element arrays and the SECOND
 * element is the one whose palette index is complemented -- `[gUnknown_0300450C]`
 * for group 0 and `[gUnknown_0300450C ^ 1]` for group 1. MovePanelsBack is this
 * function with that correspondence inverted, which is what proves the pairing
 * is the point rather than an accident of the offsets.
 *
 * The last complemented index comes out shaped differently (`eors r2, r1` and
 * the address accumulated through r8) purely because the constant 1 dies there
 * and the allocator reuses its register; there is nothing in the source to
 * match it to.
 */

#define PAL0 gUnknown_085523A8[gUnknown_0300450C]
#define PAL1 gUnknown_085523A8[gUnknown_0300450C ^ 1]

void MovePanelsForward(void)
{
    SetBg2Bg3ControlBySide(gUnknown_08551E7C[gUnknown_0300450C]);

    SetSlotSpritePriority(gUnknown_02029A10[0].entries[0].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[1].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[2].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[3].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[4].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[0].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[1].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[2].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[3].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[4].unk18, PAL1);

    SetSlotSpritePriority(gUnknown_02029668[0][0], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[0][1], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[0][2], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[0][3], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[0][4], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[1][0], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[1][1], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[1][2], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[1][3], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[1][4], PAL1);

    SetSlotSpritePriority(gUnknown_02029808[0].unk24[0], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[1], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[2], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[3], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[4], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[0], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[1], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[2], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[3], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[4], PAL1);

    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[0], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[1], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[2], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[3], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[4], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[0], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[1], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[2], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[3], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[4], PAL1);
}
asm(".global sub_08053860\n.thumb_set sub_08053860, MovePanelsForward\n");

void MovePanelsBack(void)
{
    SetBg2Bg3ControlBySide(gUnknown_0300450C);
    gUnknown_030045B0 = 1;

    SetSlotSpritePriority(gUnknown_02029A10[0].entries[0].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[1].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[2].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[3].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[0].entries[4].unk18, PAL1);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[0].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[1].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[2].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[3].unk18, PAL0);
    SetSlotSpritePriority(gUnknown_02029A10[1].entries[4].unk18, PAL0);

    SetSlotSpritePriority(gUnknown_02029668[0][0], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[0][1], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[0][2], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[0][3], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[0][4], PAL1);
    SetSlotSpritePriority(gUnknown_02029668[1][0], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[1][1], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[1][2], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[1][3], PAL0);
    SetSlotSpritePriority(gUnknown_02029668[1][4], PAL0);

    SetSlotSpritePriority(gUnknown_02029808[0].unk24[0], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[1], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[2], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[3], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[0].unk24[4], PAL1);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[0], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[1], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[2], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[3], PAL0);
    SetSlotSpritePriority(gUnknown_02029808[1].unk24[4], PAL0);

    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[0], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[1], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[2], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[3], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[0].unk02[4], PAL1);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[0], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[1], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[2], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[3], PAL0);
    SetSlotSpritePriority(gUnknown_020296B0[1].unk02[4], PAL0);
}
asm(".global sub_08053BB8\n.thumb_set sub_08053BB8, MovePanelsBack\n");
