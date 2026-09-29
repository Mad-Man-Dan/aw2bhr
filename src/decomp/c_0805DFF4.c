#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DFF4.
 * sub_0805DFF4 @ 0x0805DFF4
 */

/* AiDeliberateFootUnit @ 0x0805DFF4, 364 bytes.
 *
 * gUnknown_0816DA58 is not an object: the ROM word at 0x0816DA58 holds
 * 0x030040D8, i.e. it is agbcc's own -fforce-addr address constant for
 * gUnknown_030040D8. Naming the global honestly reproduces it and the build
 * places the .rodata word.
 *
 * Byte +9 of the record carries a THREE-BIT FIELD at bits 3..5, and that is
 * measured rather than guessed: the ROM clears it with `movs #0x39; rsbs #0;
 * ands` -- the SImode -57 that store_bit_field builds -- and sets it with the
 * same AND plus `movs #0x10; orrs`, i.e. the value 2 shifted into place. No
 * plain-u8 spelling produces either: `x &= ~0x38` on a u8 member is narrowed to
 * QImode by combine and comes out as a single `movs #0xc7`, and routing the
 * value through an `int` temporary keeps the SImode constant but puts the
 * loaded byte in the AND's destination register instead of the constant.
 * store_bit_field is the only expander that emits the constant into the
 * destination. The overlay struct lives here rather than in
 * include/unknown-globals.h because offset 9 falls inside struct
 * Unk030040D8's unk07[5], which c_0802966C.c indexes with a runtime subscript.
 *
 * p->flags and p->x are NAMED LOCALS: the ROM loads them once into two
 * callee-saved registers before the AiTryRideInsteadOfWalk branch and reuses them for the
 * `x | (y << 16)` word afterwards. That pair is why this function pushes r7.
 *
 * The trailing AiEmbarkOrFallback() belongs to the p == NULL arm as a whole, not to
 * the gUnknown_030046B8 test inside it -- the `beq` when bit 0 is clear lands on
 * the single call, and the AiListEnemyPropertyCells == 0 path reaches TWO consecutive calls
 * by falling through.
 */

struct Unk0805DFF4Rec
{
    /* 0x00 */ u8 filler_00[0x09];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 3;
               u8 unk09_6 : 2;
};

void AiDeliberateFootUnit(void)
{
    u8 buf;
    union Unk802C57CBuf v;
    struct Unit *p;
    int q;
    u8 x;
    u8 y;

    AiTryJoinUnitOnProperty();
    sub_0805D888();

    if (gUnknown_03004784[0] > (u8)(gUnknown_030040D8->unk07[3] % 100)
        || IsCoPowerActive(gUnknown_030033EC))
        AiTryAttack();

    AiGetReachBudget(&buf);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, buf, 0);
    AiAppendSiloCandidates(BuildCapturableCellList());
    q = CountUnitsWithTypeTag(1) / gUnknown_085766E0->unk04[5];
    if (q == 0)
        q = gUnknown_085766E0->unk00;
    p = AiClaimTerritoryCandidate(q, 0);
    if (p != 0)
    {
        ((struct Unk0805DFF4Rec *)gUnknown_030040D8)->unk09_3 = 0;
        x = p->flags;
        y = p->x;
        if (gUnknown_030046B8 & 1)
            AiTryRideInsteadOfWalk(x, y, 0x14, 2);
        else
            AiTryRideInsteadOfWalk(x, y, 7, 1);
        v.raw = x | (y << 16);
        AiAdvanceToward(&v);
    }
    else
    {
        if (gUnknown_030046B8 & 1)
        {
            ((struct Unk0805DFF4Rec *)gUnknown_030040D8)->unk09_3 = 2;
            AiBoardTransport();
            gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                              0x14, 0x78, 0);
            if (AiListEnemyPropertyCells(gUnknown_03003F20))
            {
                gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                                  gUnknown_030040D8->unk00, 0x78, -1);
                v.pos.unk00 = 0x270F;
                AiFindEmptyTCopter(&v);
                if (v.pos.unk00 != 0x270F)
                    AiAdvanceToward(&v);
            }
            else
            {
                AiEmbarkOrFallback();
            }
        }
        AiEmbarkOrFallback();
    }
}
asm(".global sub_0805DFF4\n.thumb_set sub_0805DFF4, AiDeliberateFootUnit\n");
